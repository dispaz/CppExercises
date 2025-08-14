#include "pch.h"
#include "D3DHack.h"
#include <iostream>

using namespace std;

void D3DHack::handleHacksOnOff(LPDIRECT3DDEVICE9 pDevice)
{
	cout << "Handling hacks on/off..." << endl;
	if (!lightHackEnabled)
	{
		if (GetAsyncKeyState(VK_NUMPAD0))
		{
			cout << "F1 pressed: Enabling light hack." << endl;
			enableLightHackDirectional(pDevice);
		}
	}

	enableLightHackAmbient(pDevice);
}

void D3DHack::enableLightHackDirectional(LPDIRECT3DDEVICE9 pDevice) 
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;;
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	light.Direction = D3DXVECTOR3(-1.0f, -0.5f, -1.0f);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	lightHackEnabled = true;
}
void D3DHack::enableLightHackAmbient(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 10, 1));
}