#include "Light.h"
DirectX::XMFLOAT4 Light::LightDir(1, 1, 1, 1);
DirectX::XMFLOAT4 Light::DirLightColor(1, 1, 1, 1);
DirectX::XMFLOAT4 Light::Ambient(1, 1, 1, 1);

void Light::Initialize()
{
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(DirectX::XM_PI);
	dir.y = -1.0f;
	dir.z = cosf(DirectX::XM_PI);
	Light::SetDirLight(dir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));
}

void Light::SetDirLight(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 color)
{
	float d = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	if (d > 0) { dir.x /= d; dir.y /= d; dir.z /= d; }
	LightDir = DirectX::XMFLOAT4(dir.x, dir.y, dir.z, 0);
	DirLightColor = DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
}

void Light::SetAmbient(DirectX::XMFLOAT3 amb)
{
	Ambient = { amb.x,amb.y,amb.z,0 };
}
