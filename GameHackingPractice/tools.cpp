#include <Windows.h>
#include <string>
#include <iostream>
#include <tlhelp32.h>
#include "GameHackingCh6.h"
#include "tools.h"

using namespace std;

HANDLE getHandleByName(wstring processName) {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32) == TRUE) {
		while (Process32Next(hProcessSnap, &pe32) == TRUE) {
			wstring binaryPath = pe32.szExeFile;
			if (binaryPath.find(processName) != wstring::npos) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				CloseHandle(hProcessSnap);
				return hProcess;
			}
		}
	}

	CloseHandle(hProcessSnap);
	return NULL;
}

DWORD getRemoteBaseAddress(HANDLE process) {
	DWORD newBase;
	HMODULE k32 = GetModuleHandle(L"kernel32.dll");
	LPVOID funcAddr = GetProcAddress(k32, "GetModuleHandleA");
	if (!funcAddr) {
		funcAddr = GetProcAddress(k32, "GetModuleHandleW");
	}

	HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcAddr, NULL, NULL, NULL);

	WaitForSingleObject(thread, INFINITE);

	GetExitCodeThread(thread, &newBase);

	CloseHandle(thread);
	return newBase;
}

DWORD getModuleBaseAddress() {
	return (DWORD)GetModuleHandle(NULL);
}

DWORD hookVF(DWORD classInst, DWORD funcIndex, DWORD newFunc)
{
	DWORD VFTable = readMemory<DWORD>(classInst);
	DWORD hookAddress = VFTable + funcIndex * sizeof(DWORD);

	auto oldProtection = protectMemory<DWORD>(hookAddress, PAGE_READWRITE);
	DWORD originalFunc = readMemory<DWORD>(hookAddress);
	writeMemory<DWORD>(hookAddress, newFunc);
	protectMemory<DWORD>(hookAddress, oldProtection);

	return originalFunc;
}

DWORD getVF(DWORD classInst, DWORD funcIndex)
{
	DWORD VFTable = readMemory<DWORD>(classInst);
	DWORD hookAddress = VFTable + funcIndex * sizeof(DWORD);
	return readMemory<DWORD>(hookAddress);
}