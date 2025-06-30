#pragma once
#include <Windows.h>

DWORD WINAPI simplePrinting(LPVOID lpParam);
void runThreads();

void loadDll(HANDLE process, const wchar_t* dllName);