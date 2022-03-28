#pragma once
#include <DirectXMath.h>

class Light
{
private:
public:

	static DirectX::XMFLOAT4 LightDir;
	static DirectX::XMFLOAT4 DirLightColor;
	static DirectX::XMFLOAT4 Ambient;

	static void Initialize();
	static void SetDirLight(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 color);
	static void SetAmbient(DirectX::XMFLOAT3 amb);

};