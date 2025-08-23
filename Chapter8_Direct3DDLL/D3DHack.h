#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class D3DHack
{
public:
	void handleHacksOnOff(LPDIRECT3DDEVICE9 pDevice);
private:
	boolean lightHackEnabled = false;
	void enableLightHackDirectional(LPDIRECT3DDEVICE9 pDevice);
	void enableLightHackAmbient(LPDIRECT3DDEVICE9 pDevice);
};