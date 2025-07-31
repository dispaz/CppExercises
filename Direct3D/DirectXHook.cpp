#include "pch.h"
#include "DirectXHook.h"
#include "tools.h"
#include <d3d9.h>

DWORD DirectXHook::locateEndScene() {
    WNDCLASSEXA wc =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        DefWindowProc,
        0L,0L,
        GetModuleHandleA(NULL),
        NULL, NULL, NULL, NULL,
        "DX", NULL
    };

    HWND hWnd = CreateWindowA(
        "DX",
        NULL,
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        600,
        600,
        GetDesktopWindow(),
        NULL,
        wc.hInstance,
        NULL
    );

    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) {
        return 0;
    }

    D3DPRESENT_PARAMETERS d3dpp;;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    LPDIRECT3DDEVICE9 device;
    HRESULT res = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &device
    );
    if (FAILED(res)) {
        return 0;
    }

    DWORD endSceneAddress = getVF((DWORD)device, 42);
	pD3D->Release();
    device->Release();
    DestroyWindow(hWnd);
    return endSceneAddress;
}

DWORD DirectXHook::getVF(DWORD classInst, DWORD funcIndex) {
    DWORD vfTable = readMemory<DWORD>(classInst);
	DWORD hookAddr = vfTable + (funcIndex * sizeof(DWORD));
    
    return readMemory<DWORD>(hookAddr);
}

unsigned char* DirectXHook::hookWithJump(DWORD hookAt, DWORD newFunc) {
    DWORD newOffset = newFunc - hookAt - 5;
	auto oldProtection = protectMemory<BYTE[5]>(hookAt, PAGE_EXECUTE_READWRITE);
	unsigned char* originals = new unsigned char[5];
    for (int i = 0; i < 5; i++) {
        originals[i] = readMemory<unsigned char>(hookAt + i);
    }
    writeMemory<BYTE>(hookAt, 0xE9);
	writeMemory<DWORD>(hookAt + 1, newOffset);
	protectMemory<BYTE[5]>(hookAt, oldProtection);
    return originals;
}

void DirectXHook::unhookWithJump(DWORD hookAt, unsigned char* originalBytes) {
	auto oldProtection = protectMemory<BYTE[5]>(hookAt, PAGE_EXECUTE_READWRITE);
    for (int i = 0; i < 5; i++) {
		writeMemory<unsigned char>(hookAt + i, originalBytes[i]);
    }
	protectMemory<BYTE[5]>(hookAt, oldProtection);
	delete[] originalBytes;
}

/**
* Write callbacks, so when hooked, we can store pointer to device
*/