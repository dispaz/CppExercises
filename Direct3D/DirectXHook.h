#include <Windows.h>
#include <d3d9.h>

class DirectXHook {
public:
private:

	DWORD locateEndScene();
	DWORD getVF(DWORD classInst, DWORD funcIndex);
	unsigned char* hookWithJump(DWORD hookAt, DWORD newFunc);
	void unhookWithJump(DWORD hookAt, unsigned char* originalBytes);
};
