#include <Windows.h>
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
	if(WriteProcessMemory(process, address, &value, sizeof(T), NULL)) {
		// Successfully written
	} else {
		
	}
}

template <typename T>
T readValueAtAddress(LPVOID address) {
	return *((T*)address);
}

void temp() {
	int variable = 12345;
	LPVOID address = &variable;
	
	cout << "Variable address: " << address << endl;
	int readVar = readValueAtAddress<int>(address);
	cout << "Variable value: " << readVar << endl;
	
	cin.get();
}
