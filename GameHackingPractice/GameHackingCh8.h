#include <Windows.h>
#include "common.h"
#include <iostream>

using namespace std;

class Chapter8Runner : public IChapterRunner {  
public:  
    void run() override;  

private:
    void doNOPFunction();
    void callHookingFunction();
    LPVOID writeShellCodeToProc(HANDLE proc);
    void hookedFunction2();
    LPVOID allocateAndWriteStringToProc(HANDLE proc, const wchar_t* str);
};