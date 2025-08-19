#include "common.h"
#include <Windows.h>

class Chapter8VFRunner : public IChapterRunner {
public:
	void run() override;
private:
	DWORD hookVirtualFun(DWORD classInstance, DWORD funId, DWORD newFunc);
};

class BaseClassVFHook {
public:
	virtual void virtualFun(DWORD arg1) = 0;
};

class DerivedClassVFHook : public BaseClassVFHook {
public:
	void virtualFun(DWORD arg1) override;
	void fun();
};