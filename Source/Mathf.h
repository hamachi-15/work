#pragma once
#include <DirectXMath.h>

/// <summary>
/// 計算関数クラス
/// </summary>
class Mathf
{
public:
	// 線形補完
	static float Lerp(float a, float b, float t);
	
	// 指定範囲のランダム値を算出
	static float RandomRange(float min, float max);
	
	// ラジアン値へ変換
	static float ConvartToRadian(float dogree);

	// 平面からの最近値算出
	static void NegaCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius);
	
	// 平面からの最遠値算出
	static void PosiCalculate(DirectX::XMFLOAT3& anser, DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& radius);
};