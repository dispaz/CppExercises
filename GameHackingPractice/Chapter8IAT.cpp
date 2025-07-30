#include <Windows.h>
#include <iostream>
#include "Chapter8IAT.h"
#include "tools.h"

using namespace std;

typedef VOID (WINAPI _sleepFun)(DWORD ms);
_sleepFun* sleepFun;

VOID WINAPI sleepHook(DWORD ms) {
	if (ms > 100) 
	{
		cout << "Sleep hook worked. Denied sleep for " << ms << " milliseconds." << endl;
	}
	else
	{
		sleepFun(ms);
	}
}

void Chapter8IATRunner::run() {
	cout << "Chapter 8 IAT Hooking" << endl;
	sleepFun = (_sleepFun*)hookIAT("Sleep", (DWORD)&sleepHook);
	Sleep(300);
	cout << "Sleep end" << endl;
}

DWORD Chapter8IATRunner::hookIAT(const char* functionName, DWORD newFunctionAddress) {
	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);

	auto dosHeader = pointMemory<IMAGE_DOS_HEADER>(baseAddress);
	if (dosHeader->e_magic != 0x5A4D)
	{
		cout << "Invalid DOS header." << endl;
		return 1;
	}
	
	auto optHeader = pointMemory<IMAGE_OPTIONAL_HEADER>(baseAddress + dosHeader->e_lfanew + 24);
	if (optHeader->Magic != 0X10B) 
	{
		cout << "Invalid PE header." << endl;
		return 1;
	}
	
	auto IAT = optHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if(IAT.Size == 0 || IAT.VirtualAddress == 0) 
	{
		cout << "No IAT found." << endl;
		return 1;
	}

	IMAGE_IMPORT_DESCRIPTOR* importDescr = pointMemory<IMAGE_IMPORT_DESCRIPTOR>(baseAddress + optHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	while (importDescr->FirstThunk) 
	{
		int n = 0;
		IMAGE_THUNK_DATA* thunk = pointMemory<IMAGE_THUNK_DATA>(baseAddress + importDescr->OriginalFirstThunk);
		char* funName = pointMemory<char>(baseAddress + thunk->u1.AddressOfData + 2);
		while (thunk->u1.Function)
		{
			char* importFunctionName = pointMemory<char>(baseAddress + thunk->u1.AddressOfData + 2);
			cout << "Import function: " << importFunctionName << endl;
			if(strcmp(importFunctionName, functionName) == 0) 
			{
				auto vfTable = pointMemory<DWORD>(baseAddress + importDescr->FirstThunk);
				cout << "Found function: " << importFunctionName << " at address:" << hex << vfTable << endl;
				DWORD original = vfTable[n];

				auto oldProtection = protectMemory<DWORD>((DWORD)&vfTable[n], PAGE_READWRITE);
				vfTable[n] = newFunctionAddress;
				protectMemory<DWORD>((DWORD)&vfTable[n], oldProtection);

				return original;
			}
			n++;
			thunk++;
		}
		importDescr++;
	}

	cout << "Base address: " << hex << baseAddress << endl;
	return 0;
}