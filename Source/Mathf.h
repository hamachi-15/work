#pragma once

class Mathf
{
public:
	// 線形補完
	static float Lerp(float a, float b, float t);
	// 指定範囲のランダム値を算出
	static float RandomRange(float min, float max);
	// ラジアン値へ変換
	static float ConvartToRadian(float dogree);

};