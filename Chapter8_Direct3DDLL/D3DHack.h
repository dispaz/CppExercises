#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class D3DHack
{
public:
	void handleHacksOnOff(LPDIRECT3DDEVICE9 pDevice);
	void handleWallhack(LPDIRECT3DDEVICE9 pDevice);
private:
	bool lightHackEnabled = false;
	bool wallhackEnabled = false;
	void enableLightHackDirectional(LPDIRECT3DDEVICE9 pDevice);
	void enableLightHackAmbient(LPDIRECT3DDEVICE9 pDevice);
	void enableWallhack(LPDIRECT3DDEVICE9 pDevice);
};