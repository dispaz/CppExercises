#include "GameHackingCh8.h"
#include <iostream>
#include <Windows.h>
#include "tools.h"

using namespace std;

const wstring MODULE_NAME = L"SimpleProgram.exe";
const wchar_t* msgText = L"Function hooked";
const wchar_t* captionText = L"HOOKED!";

void Chapter8Runner::run() {
	cout << "###### Chapter 8 ######" << endl;
	callHookingFunction();	
}

void Chapter8Runner::doNOPFunction() {
	cout << "Chapter 8 work in progress..." << endl;

	DWORD targetNOPAdd = 0x00412550;

	const int SIZE = 5; // Size of the NOP instruction to write

	
	HANDLE proc = getHandleByName(MODULE_NAME);

	if (proc == NULL) {
		cout << "Process not found!" << endl;
		return;
	}

	auto oldProtect = protectMemoryAPI<BYTE[SIZE]>(proc, targetNOPAdd, PAGE_EXECUTE_READ);
	LPVOID targetNOP = (LPVOID)targetNOPAdd;

	writeNOP<SIZE>(proc, targetNOPAdd);

	cout << "NOPs written successfully." << endl;
}

void Chapter8Runner::callHookingFunction() {
	
	HANDLE proc = getHandleByName(MODULE_NAME);
	if (proc == NULL) {
		cout << "Process not found!" << endl;
		return;
	}
	auto msgAddrToWrite = allocateAndWriteStringToProc(proc, msgText);
	auto captionAddrToWrite = allocateAndWriteStringToProc(proc, captionText);
	cout << "Message and caption written to process memory." << endl;

	BYTE hookCode[] = {
		0x8B, 0xF4, // mov esi, esp
		0x68, 0x00, 0x04, 0x00, 0x00, // push 40000 (type of message box) - 4th
		0x68, 0x00, 0x00, 0x00, 0x00, // push address caption text - 3rd
		0x68, 0x00, 0x00, 0x00, 0x00, // push address of msg text - 2nd
		0x6A, 0x00, // push 0 (wnd) - 1st
		0xFF, 0x15, 0xA0, 0x15, 0xAF, 0x75, // call dword ptr ds:MessageBoxA [0x75AF15A0]
		0x3B, 0xF4 // cmp esi, esp
	};

	memcpy(&hookCode[8], &captionAddrToWrite, 4); // Copy the address of msgText into the hook code
	memcpy(&hookCode[12], &msgAddrToWrite, 4); // Copy the address of msgText into the hook code

	LPVOID hookAddr = VirtualAllocEx(proc, NULL, sizeof(hookCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(hookAddr == NULL) {
		cout << "VirtualAllocEx failed!" << endl;
		return;
	}

	if(WriteProcessMemory(proc, hookAddr, hookCode, sizeof(hookCode), NULL) == 0) {
		cout << "WriteProcessMemory failed!" << endl;
		VirtualFreeEx(proc, hookAddr, 0, MEM_RELEASE);
		return;
	}
}

void Chapter8Runner::hookedFunction2() {
	MessageBoxA(NULL, "Function hooked", "", MB_OK | MB_TOPMOST);
}

LPVOID Chapter8Runner::allocateAndWriteStringToProc(HANDLE proc, const wchar_t* str) {
	SIZE_T strSize = wcslen(str) * sizeof(wchar_t) + sizeof(wchar_t); // +1 for null terminator
	LPVOID strAddr = VirtualAllocEx(proc, NULL, strSize, MEM_COMMIT, PAGE_READWRITE);
	if (strAddr == NULL) {
		cout << "VirtualAllocEx failed!" << endl;
		return NULL;
	}
	if (WriteProcessMemory(proc, strAddr, str, strSize, NULL) == 0) {
		cout << "WriteProcessMemory failed!" << endl;
		VirtualFreeEx(proc, strAddr, 0, MEM_RELEASE);
		return NULL;
	}
	return strAddr;
}