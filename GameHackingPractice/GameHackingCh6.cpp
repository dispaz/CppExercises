#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;

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

DWORD rebase(DWORD address, DWORD newbase) {
	return address - 0x400000 + newbase;
}