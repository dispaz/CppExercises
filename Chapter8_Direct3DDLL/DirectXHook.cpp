#include "pch.h"
#include "DirectXHookCallbacks.h"
#include "DirectXHook.h"
#include "tools.h"
#include <d3d9.h>

DirectXHook* DirectXHook::instance = nullptr;
LPDIRECT3DDEVICE9 DirectXHook::hookedDevice = NULL;
unsigned char* DirectXHook::originalEndSceneCode = NULL;
_endScene DirectXHook::originalEndScene = NULL;
DWORD DirectXHook::endSceneAddress = NULL;

__declspec(naked) void endSceneTrampoline() {
    __asm {
        MOV EAX, DWORD PTR SS : [ESP + 0x4]
        PUSH EAX
        CALL reportInitEndScene
        JMP EAX
    }
}

void DirectXHook::init() {
	cout << "Initializing DirectX Hook..." << endl;
    while(!GetModuleHandleA("d3d9.dll")) {
        Sleep(100);
	}
    DirectXHook::endSceneAddress = locateEndScene();

    if (endSceneAddress) {
        DirectXHook::getInstance()->originalEndSceneCode = hookWithJump(endSceneAddress, (DWORD)&endSceneTrampoline);
    }
}

DWORD DirectXHook::locateEndScene() {
	cout << "Locating EndScene..." << endl;
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

    RegisterClassExA(&wc);
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

unsigned char* DirectXHook::hookWithJump(DWORD hookAt, DWORD newFunc) {
	cout << "Running hookWithJump..." << endl;

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
    cout << "Running unhookWithJump..." << endl;
	auto oldProtection = protectMemory<BYTE[5]>(hookAt, PAGE_EXECUTE_READWRITE);
    for (int i = 0; i < 5; i++) {
		writeMemory<unsigned char>(hookAt + i, originalBytes[i]);
    }
	protectMemory<BYTE[5]>(hookAt, oldProtection);
	delete[] originalBytes;
}

DWORD DirectXHook::initHookCallback(LPDIRECT3DDEVICE9 device) {
	cout << "Running initHookCallback..." << endl;
    DirectXHook::hookedDevice = device;
    while (DirectXHook::originalEndSceneCode == nullptr) {}
    unhookWithJump(DirectXHook::endSceneAddress, DirectXHook::originalEndSceneCode);
	this->placeHooks();
    return endSceneAddress;
}

DX_API DirectXHook::endSceneHookCallback(LPDIRECT3DDEVICE9 device) {
    cout << "Running endSceneHookCallback..." << endl;
	auto result = originalEndScene(device);
    return result;
}

void DirectXHook::placeHooks() {
    cout << "Placing hooks..." << endl;
    DWORD orig = hookVF((DWORD)DirectXHook::hookedDevice, 42, (DWORD)&endSceneCallback);
    if (orig != (DWORD)&endSceneCallback) 
    {
        originalEndScene = (_endScene)orig;
    }
	//hack.handleHacksOnOff(hookedDevice);
    // Here you can place additional hooks or perform other initialization tasks.
    // For example, you might want to hook other DirectX functions or set up logging.
    
    /*
	* Hooks can be placed here for other DirectX functions.
    */
}
