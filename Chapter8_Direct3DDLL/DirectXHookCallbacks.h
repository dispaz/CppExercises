#pragma once
#include "pch.h"
#include "DirectXHook.h"


DWORD __stdcall reportInitEndScene(LPDIRECT3DDEVICE9 device);



DX_API endSceneCallback(LPDIRECT3DDEVICE9 device);