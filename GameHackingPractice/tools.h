#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

template <typename T>
T* pointMemory(DWORD address) 
{
	return ((T*)address);
}

template <typename T>
T readMemory(DWORD address) {
	return *((T*)address);
}

template <typename T>
T readMemoryAPI(HANDLE process, LPVOID address) {
	T value;
	if (ReadProcessMemory(process, address, &value, sizeof(T), NULL)) {
		return value;
	}
	return NULL;
}

template <typename T>
void writeMemory(DWORD address, T value) {
	*((T*)address) = value;
}

template <typename T>
void writeMemoryAPI(HANDLE process, LPVOID address, T value) {
	if (WriteProcessMemory(process, address, &value, sizeof(T), NULL)) {
		// Successfully written
		cout << "Value written successfully." << endl;
	}
	else {
		cout << "Failed to write value." << endl;
	}
}

template <typename T>
DWORD protectMemory(DWORD address, DWORD protection) {
	DWORD oldProt;
	VirtualProtect((LPVOID)address, sizeof(T), protection, &oldProt);
	return oldProt;
}

template <typename T>
DWORD protectMemoryAPI(HANDLE proc, DWORD address, DWORD protection) {
	DWORD oldProt;
	VirtualProtectEx(proc, (LPVOID)address, sizeof(T), protection, &oldProt);
	return oldProt;
}

template <int SIZE>
void writeNOP(HANDLE proc, DWORD address) {
	auto oldProtection = protectMemoryAPI<BYTE[SIZE]>(proc, address, PAGE_EXECUTE_READWRITE);
	for (int i = 0; i < SIZE; i++) {
		writeMemoryAPI<BYTE>(proc, (LPVOID)(address + i), 0x90);
	}
	protectMemoryAPI<BYTE[SIZE]>(proc, address, oldProtection);
}

HANDLE getHandleByName(std::wstring moduleName);
DWORD getRemoteBaseAddress(HANDLE process);

DWORD rebase(DWORD address, DWORD newBase);
DWORD getModuleBaseAddress();
HANDLE getHandleByName(std::wstring moduleName);
DWORD getRemoteBaseAddress(HANDLE process);