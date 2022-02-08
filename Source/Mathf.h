#pragma once
#include <DirectXMath.h>

// 前方宣言
class Model;

/// <summary>
/// 計算関数クラス
/// </summary>
class Mathf
{
public:
	// ベクトル算出
	static DirectX::XMVECTOR ReturnVectorSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);
	static DirectX::XMFLOAT3 ReturnFloatSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);
	// 正規化されたベクトル算出
	static DirectX::XMFLOAT3 ReturnNormalizeFloatSubtract(const DirectX::XMFLOAT3& subtract1, const DirectX::XMFLOAT3& subtract2);

	// ベクトルの長さ算出
	static float VectorLength(const DirectX::XMVECTOR& vector);
	static float VectorLength(const DirectX::XMFLOAT3& vector);

	// 角度から前方向を算出
	static DirectX::XMVECTOR ReturnVectorFront(const DirectX::XMFLOAT3& angle);
	static DirectX::XMFLOAT3 ReturnFloatFront(const DirectX::XMFLOAT3& angle);

	// 内積算出
	static DirectX::XMVECTOR ReturnVectorDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2);
	static float ReturnFloatDot(const DirectX::XMVECTOR& vector1, const DirectX::XMVECTOR& vector2);

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

	// モデルのノード座標取得
	static void GetNodePosition(const char* node_name, DirectX::XMFLOAT3& node_position, Model* model);
};