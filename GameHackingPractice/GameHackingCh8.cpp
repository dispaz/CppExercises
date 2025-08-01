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

LPVOID Chapter8Runner::writeShellCodeToProc(HANDLE proc) {
	BYTE hookCode[] = {
		0x8B, 0xF4, // mov esi, esp
		0x68, 0x00, 0x00, 0x04, 0x00, // push 40000 (type of message box) - 4th
		0x68, 0x00, 0x00, 0x00, 0x00, // push address caption text - 3rd
		0x68, 0x00, 0x00, 0x00, 0x00, // push address of msg text - 2nd
		0x6A, 0x00, // push 0 (wnd) - 1st
		0xFF, 0x15, 0x34, 0xD1, 0x41, 0x00, // call dword ptr ds:MessageBoxA [0x75AF15A0]
		0x3B, 0xF4, // cmp esi, esp
		0xC3, //retn
		0x90, 0x90
	};

	int hookCodeSize = sizeof(hookCode);
	int msgTextSize = wcslen(msgText) * sizeof(wchar_t) + sizeof(wchar_t); // +1 for null terminator
	int captionTextSize = wcslen(captionText) * sizeof(wchar_t) + sizeof(wchar_t); // +1 for null terminator
	int fullLen = hookCodeSize + msgTextSize + captionTextSize;

	cout << "Hook code size: " << hookCodeSize << "\nmsgTextSize: " << msgTextSize << "\ncaptionTextSize: " << captionTextSize << endl;

	LPVOID allocatedAddr = VirtualAllocEx(proc, NULL, fullLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocatedAddr == NULL) {
		cout << "Error allocating memory" << endl;
		return NULL;
	}
	LPVOID msgTextAddr = (LPVOID)((DWORD)allocatedAddr + captionTextSize);
	LPVOID hookCodeAddr = (LPVOID)((DWORD)msgTextAddr + msgTextSize);

	cout << "Allocated memory at: " << hex << allocatedAddr << "\nMessage text address: " << hex << msgTextAddr << "\nHook code address: " << hex << hookCodeAddr << endl;

	memcpy(&hookCode[8], &allocatedAddr, 4); // Copy the address of msgText into the hook code
	memcpy(&hookCode[13], &msgTextAddr, 4); // Copy the address of msgText into the hook code

	if (WriteProcessMemory(proc, msgTextAddr, msgText, msgTextSize, NULL) == 0) {
		cout << "WriteProcessMemory failed!" << endl;
		return NULL;
	}

	if (WriteProcessMemory(proc, allocatedAddr, captionText, captionTextSize, NULL) == 0) {
		cout << "WriteProcessMemory failed!" << endl;
		return NULL;
	}

	if (WriteProcessMemory(proc, hookCodeAddr, hookCode, sizeof(hookCode), NULL) == 0) {
		cout << "WriteProcessMemory failed!" << endl;
		return NULL;
	}
	return hookCodeAddr;
}

void Chapter8Runner::callHookingFunction() {
	
	HANDLE proc = getHandleByName(MODULE_NAME);
	if (proc == NULL) {
		cout << "Process not found!" << endl;
		return;
	}

	LPVOID funAddr = writeShellCodeToProc(proc);
	if (funAddr == NULL) {
		cout << "Something went wrong..." << endl;
	}
	BYTE callCode[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 }; // call instruction
	
	DWORD callAddr = 0x00411DAC;
	DWORD newAddr = (DWORD)funAddr - (DWORD)callAddr - 5;
	memcpy(&callCode[1], &newAddr, 4);

	if (WriteProcessMemory(proc, (LPVOID)callAddr, callCode, sizeof(callCode), NULL) == 0) {
		cout << "Error WriteProcessMemory" << endl;
		return;
	}
	cout << "Call hooking succeeed" << endl;
}

void Chapter8Runner::hookedFunction2() {
	MessageBoxW(NULL, msgText, captionText, MB_OK | MB_TOPMOST);
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