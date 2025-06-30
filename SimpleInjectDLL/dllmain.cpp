// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <Windows.h>

DWORD WINAPI simplePrinting(LPVOID lpParam) {
    int counter = 1000;
    while (counter > 0) {
        printf("Thread counter: %d\n", counter);
        counter--;
        Sleep(250);
    }
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(NULL, "DLL Attached!\n", "Game Hacking", MB_OK | MB_TOPMOST);
        
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)simplePrinting, 0, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        std::cout << "DLL detached" << std::endl;
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

