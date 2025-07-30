#include "common.h"
#include <Windows.h>

class Chapter8IATRunner : public IChapterRunner {
private:
	DWORD hookIAT(const char* functionName, DWORD newFunctionAddress);
public:
	void run() override;
};