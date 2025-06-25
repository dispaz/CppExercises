#pragma once

#include <windows.h>

class Injector {
public:
	DWORD getPIDByName(std::wstring processName);
	HANDLE getHandle(DWORD PID);
	void injectCode(HANDLE hProcess, LPVOID funcn);
};