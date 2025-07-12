#include <Windows.h>
#include <string>

void temp();
DWORD rebase(DWORD address, DWORD newBase);
DWORD getModuleBaseAddress();
HANDLE getHandleByName(std::wstring moduleName);
DWORD getRemoteBaseAddress(HANDLE process);