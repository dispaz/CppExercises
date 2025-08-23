#pragma once

#include <Windows.h>
#include "D3DHack.h"
#include <d3d9.h>
#include <vector>

#define DX_API HRESULT WINAPI

typedef HRESULT(WINAPI* _endScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* _drawPrimitive)(LPDIRECT3DDEVICE9 pDevice, DWORD primType, UINT StartVertex, UINT PrimitiveCount);
//typedef _drawFrameCallback(LPDIRECT3DDEVICE9);

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

	void drawText(int x, int y, D3DCOLOR color, const char* text, ...);

	DWORD initHookCallback(LPDIRECT3DDEVICE9 device);
	DX_API endSceneHookCallback(LPDIRECT3DDEVICE9 device);
	DX_API drawPrimitiveHookCallback(LPDIRECT3DDEVICE9 device, DWORD primType, UINT StartVertex, UINT PrimitiveCount);
	DX_API drawIndexedPrimitiveHookCallback(LPDIRECT3DDEVICE9 device, DWORD primType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
private:
	DirectXHook(void) 
	{
		hack = D3DHack();
	};
	~DirectXHook(void) {};

	static DirectXHook* instance;
	static LPDIRECT3DDEVICE9 hookedDevice;
	static unsigned char* originalEndSceneCode;
	static _endScene originalEndScene;
	static _drawPrimitive origDrawPrimitive;

	LPD3DXFONT font;

	static DWORD endSceneAddress;
	D3DHack hack;

	DWORD locateEndScene();
	void placeHooks();
};

struct VFHookInfo
{
	VFHookInfo(DWORD _index, DWORD _callback, DWORD* _original) : index(_index), callback(_callback), original(_original) {}
	DWORD index, callback;
	DWORD* original;
};