#pragma once

#include <Windows.h>
#include <d3d9.h>

class DirectXHook {
public:
	static DirectXHook* getInstance() {
		if (!DirectXHook::instance) {
			DirectXHook::instance = new DirectXHook();
		}
		return DirectXHook::instance;
	}

	static void deleteInstance()
	{
		if (DirectXHook::instance)
		{
			delete DirectXHook::instance;
			DirectXHook::instance = nullptr;
		}
	}

	void init();
	unsigned char* hookWithJump(DWORD hookAt, DWORD newFunc);
	void unhookWithJump(DWORD hookAt, unsigned char* originalBytes);
	DWORD initHookCallback(LPDIRECT3DDEVICE9 device);
private:
	DirectXHook(void) {};
	~DirectXHook(void) {};

	static DirectXHook* instance;
	static LPDIRECT3DDEVICE9 hookedDevice;
	static unsigned char* originalEndSceneCode;
	static DWORD endSceneAddress;
	DWORD locateEndScene();
	DWORD getVF(DWORD classInst, DWORD funcIndex);
	void placeHooks();
};
