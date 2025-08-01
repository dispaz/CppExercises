// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "DirectXHook.h"
#include <windows.h>
#include <d3d9.h>
//#include <d3dx9.h

DWORD WINAPI LoopFunction(LPVOID lpParam) {
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

