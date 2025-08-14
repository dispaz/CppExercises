// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "DirectXHook.h"
#include <windows.h>
#include <d3d9.h>
#include <iostream>

using namespace std;
//#include <d3dx9.h
void WINAPI CreateConsoleWindow();

DWORD WINAPI LoopFunction(LPVOID lpParam) {
    CreateConsoleWindow(); // Create a console window for debugging
	DirectXHook::getInstance()->init();
    return 0;
}

BOOL WINAPI DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
		CreateThread(0, 0, LoopFunction, 0, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void WINAPI CreateConsoleWindow() {
    AllocConsole();
    FILE* fp;
    if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
    {
		MessageBoxA(nullptr, "Failed to redirect stdout", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    if (freopen_s(&fp, "CONIN$", "r", stdin) != 0) 
    {
        MessageBoxA(nullptr, "Failed to redirect stdin", "Error", MB_OK | MB_ICONERROR);
		return;
    }
	cout << "Console window created!" << endl;
}

