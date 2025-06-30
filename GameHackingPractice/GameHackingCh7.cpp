#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

void runThreads(LPVOID func) {
    HANDLE threads[3];
    const DWORD count = 3;
    for (int i = 0; i < count; i++) {
        int* threadNumber = new int(i); // Allocate memory for thread number
        auto thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, threadNumber, 0, NULL);
        threads[i] = thread;
    }

    for (int i = 0; i < 10; i++) {
        printf("Main thread: %d \n", i);
        Sleep(500);
    }

    WaitForMultipleObjects(count, threads, TRUE, INFINITE);
    for (int i = 0; i < count; i++) {
        CloseHandle(threads[i]);
    }
}

bool dllFileExists(const wchar_t* dllPath) {
    DWORD attrs = GetFileAttributesW(dllPath);
    return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

void loadDll(HANDLE process, const wchar_t* dllName) {
    if (!dllFileExists(dllName)) {
        cout << dllName << " - was not found" << endl;
        return;
    }

    int nameLen = wcslen(dllName) + 1;
    LPVOID remoteStr = VirtualAllocEx(process, NULL, nameLen * 2, MEM_COMMIT, PAGE_EXECUTE);

    WriteProcessMemory(process, remoteStr, dllName, nameLen * 2, NULL);

    HMODULE k32 = GetModuleHandleA("kernel32.dll");
    LPVOID funcAddr = GetProcAddress(k32, "LoadLibraryW");

    HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcAddr, remoteStr, NULL, NULL);
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}