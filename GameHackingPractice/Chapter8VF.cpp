#include "Chapter8VF.h"
#include <iostream>
#include "tools.h"

using namespace std;

DWORD originalVF;
DWORD __stdcall newFunction(DWORD arg1) {
	static DWORD _this, _ret;
	__asm MOV _this, ECX // Get the 'this' pointer

	cout << "newFunction called with arg1: " << arg1 << endl;

	__asm {
		PUSH 1
		MOV ECX, _this
		CALL [originalVF]
		MOV _ret, EAX
	}
	cout << "Finished newFunction" << endl;
	__asm MOV ECX, _this
	return _ret;
}

void Chapter8VFRunner::run() {
	cout << "Running Chapter8VFRunner" << endl;
	DerivedClassVFHook* instance = new DerivedClassVFHook();

	originalVF = hookVF((DWORD)instance, 0, (DWORD)&newFunction);
	instance->virtualFun(0);
	delete instance;
}

DWORD Chapter8VFRunner::hookVF(DWORD classInstance, DWORD funId, DWORD newFunc) {
	DWORD VFTable = readMemory<DWORD>(classInstance);
	DWORD hookAddr = VFTable + funId * sizeof(DWORD);
	cout << "Class instance address: " << hex << classInstance
		<< "\nVFTable address: " << hex << VFTable
		<< "\nvirtualFunAddr: " << hex << hookAddr << endl;

	auto oldProt = protectMemory<DWORD>(hookAddr, PAGE_READWRITE);
	DWORD originalFunction = readMemory<DWORD>(hookAddr);
	writeMemory<DWORD>(hookAddr, newFunc);
	protectMemory<DWORD>(hookAddr, oldProt);

	return originalFunction;
}

void DerivedClassVFHook::virtualFun(DWORD arg1) {
	cout << "DerivedClassVFHook::virtualFun called" << endl;
}