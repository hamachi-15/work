//--------------------------------------------
//	グローバル変数
//--------------------------------------------
cbuffer CBBuffer : register(b0)
{
	float	timer;
	int		number;
	float2	dummy;
};

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------
struct VSInput
{
	float3 position : POSITION;
	float4 color    : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal   : NORMAL;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

static const float PI = 3.1415f;

//--------------------------------------------
//	箱
//--------------------------------------------
float Box(float2 p, float2 size) {
	float2 corner = float2(0.5f, 0.5) - size * 0.5;
	float2 uv = step(corner, p);
	uv *= step(corner, 1 - p);
	return uv.x * uv.y;
}

//--------------------------------------------
//	円
//--------------------------------------------
float Circle(float2 p, float r) {
	float2	vec = p - float2(0.5f, 0.5f);
	float	len = length(vec);
	return step(len, r);
}

//--------------------------------------------
//	輪
//--------------------------------------------
float Ring(float2 p, float size, float w) {
	p = p * 2.0f - 1.0f;
	size *= 2;
	return step(0.9999f, 1.0f - abs(length(p) - size) + w);
}

//--------------------------------------------
//	放射線
//--------------------------------------------
float Radiation(float2 p, float num, float t) {
	p = p - (float2)0.5f;
	float angle = atan2(p.y, p.x);
	return step(t, sin(angle * num));
}

//--------------------------------------------
//	放射線
//--------------------------------------------
float Radiation(float2 p, float num, float t, float angle_offset) {
	p = p - (float2)0.5f;
	float angle = atan2(p.y, p.x);
	return step(t, sin((angle + angle_offset) * num));
}
