#include <Windows.h>
#include "tools.h"
#include <iostream>
#include <TlHelp32.h>

using namespace std;


void temp() {
	int variable = 12345;
	LPVOID address = &variable;
	
	cout << "Variable address: " << address << endl;
	int readVar = readMemory<int>((DWORD)address);
	cout << "Variable value: " << readVar << endl;
	
	cin.get();
}

DWORD rebase(DWORD address, DWORD newbase) {
	return address - 0x400000 + newbase;
}