#include "tools.h"
#include <Windows.h>

template <int SIZE>
void writeNOP(DWORD address) {
	auto oldProtection = protectMemory<BYTE[SIZE]>(address, SIZE, PAGE_EXECUTE_READWRITE);
	for (int i = 0; i < SIZE; i++) {
		*((BYTE*)address) = 0x90;
	}
	protectMemory<BYTE[SIZE]>(address, SIZE, oldProtection);
}