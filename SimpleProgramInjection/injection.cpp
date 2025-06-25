#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include "injection.h"

using namespace std;

void functionToCall() {
	int i = 0;
	while (i < 1000) {
		Sleep(1000); // Sleep for 1 second
		i++;
		cout << "Function called " << i << " times." << endl;
	}
	cin.get();
}

DWORD Injector::getPIDByName(wstring processName) {
	DWORD PID = -1;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(Process32First(hSnapshot, &entry) == TRUE) {
		while(Process32Next(hSnapshot, &entry) == TRUE) {
			wstring binaryPath = entry.szExeFile;
			if(binaryPath.find(processName) != wstring::npos) {
				PID = entry.th32ProcessID;
				break;
			}
		}
	}
	return PID;
}

HANDLE Injector::getHandle(DWORD PID) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if(hProcess == NULL) {
		cout << "Error opening process: " << GetLastError() << endl;
		return NULL;
	}
	return hProcess;
}

unsigned char shellcode2Param[] = {
		0xFF, 0x74, 0x24, 0x04, // PUSH DWORD PTR[ESP+0x4]
		0x68, 0x00, 0x00, 0x00, 0x00, // PUSH 0
		0xB8, 0x00, 0x00, 0x00, 0x00, // MOV EAX, 0x0
		0xFF, 0xD0, // CALL EAX
		0x83, 0xC4, 0x08, // ADD ESP, 0x08
		0xC3 // RETN
};

signed char shellcode[] = {
		//0xFF, 0x74, 0x24, 0x04, // PUSH DWORD PTR[ESP+0x4]
		//0x68, 0x00, 0x00, 0x00, 0x00, // PUSH 0
		0xB8, 0x00, 0x00, 0x00, 0x00, // MOV EAX, 0x0
		0xFF, 0xD0, // CALL EAX
		0x83, 0xC4, 0x08, // ADD ESP, 0x08
		0xC3 // RETN
};

void Injector::injectCode(HANDLE hProcess, LPVOID func) {
	memcpy(&shellcode[1], &func, 4);
	cout << "Shellcode size: " << sizeof(shellcode) << endl;
	LPVOID remoteCave = VirtualAllocEx(hProcess, NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE);
	cout << "Remote cave allocated at: " << remoteCave << endl;

	if(WriteProcessMemory(hProcess, remoteCave, shellcode, sizeof(shellcode), NULL) == 0) {
		cout << "Error writing to process memory: " << GetLastError() << endl;
		return;
	}

	try {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteCave, NULL, NULL, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	catch (const exception& e) {
		cout << "Exception occurred: " << e.what() << endl;
	}
}

