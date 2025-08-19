#include "pch.h"
#include "tools.h"

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