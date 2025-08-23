#include "pch.h"
#include "DirectXHookCallbacks.h"
#include <iostream>

using namespace std;

DX_API endSceneCallback(LPDIRECT3DDEVICE9 device) {
    cout << "Running endSceneCallback..." << endl;
    return DirectXHook::getInstance()->endSceneHookCallback(device);
}

DX_API drawPrimitiveCallback(LPDIRECT3DDEVICE9 device, DWORD primType, UINT StartVertex, UINT PrimitiveCount)
{
    return DirectXHook::getInstance()->drawPrimitiveHookCallback(device, primType, StartVertex, PrimitiveCount);
}
DWORD __stdcall reportInitEndScene(LPDIRECT3DDEVICE9 device) {
    cout << "Running reportInitEndScene..." << endl;
    return DirectXHook::getInstance()->initHookCallback(device);
}