#pragma once
#include "Shader.h"
// 
enum class PrimitiveType
{
	Encount = 3
};
//***************************************
// 
// 2Dプリミティブクラス
//
//***************************************
class Primitive : public Shader
{
public:
	// コンストラクタ
	Primitive(ID3D11Device* device);
	~Primitive() override{}

	// 名前取得
	const char* GetShaderName()const override { return "2DPrimitive"; }
	
	// 描画開始処理
	void Begin(ID3D11DeviceContext* cotnext, PrimitiveContext& number)override;

	// 描画終了処理
	void End(ID3D11DeviceContext* context)override;
private:
	// コンスタントバッファ
	struct ConstantBuffer
	{
		float timer;
		int number;
		int dummy1;
		int dummy2;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer = nullptr;

};