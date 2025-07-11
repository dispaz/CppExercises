#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

template <typename T>
T readMemoryAPI(HANDLE process, LPVOID address) {
	T value;
	SIZE_T bytesRead;
	if (ReadProcessMemory(process, address, &value, sizeof(T), NULL)) {
		return value;
	}
	return NULL;
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
T readValueAtAddress(LPVOID address) {
	return *((T*)address);
}

template <typename T>
DWORD protectMemory(DWORD address, DWORD protection) {
	DWORD oldProt;
	VirtualProtect((LPVOID)address, sizeof(T), protection, &oldProt);
	return oldProt;
}

DWORD getModuleBaseAddress();
HANDLE getHandleByName(std::wstring moduleName);
DWORD getRemoteBaseAddress(HANDLE process);