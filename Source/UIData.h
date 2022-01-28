#pragma once
#include <memory>
#include <DirectXMath.h>
#include "ResourceManager.h"

class Charactor;

struct UIParameter
{
	std::shared_ptr<Charactor>	charactor = nullptr;
	const char*					filename = "";
	std::string					name = "";
	DirectX::XMFLOAT2			position = {};
	DirectX::XMFLOAT2			scale = {};
	float						angle = 0.0f;
};
//***********************************
// 
// UIデータクラス
// 
//***********************************
class UIData
{
public:
	// コンストラクタ
	UIData(const UIParameter& parameter) :
		name(parameter.name), position(parameter.position), angle(parameter.angle),charactor(parameter.charactor),
		texture(ResourceManager::Instance().LoadTexture(parameter.filename)), scale(parameter.scale), 
		size({ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight())}) {}

	~UIData(){}

	// キャラクタコンポーネント設定
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }

	// キャラクタコンポーネント取得
	std::shared_ptr<Charactor> GetCharactor() const { return charactor; }

	// 描画座標設定
	void SetPosition(DirectX::XMFLOAT2 position) { this->position = position; }

	// 描画座標取得
	const DirectX::XMFLOAT2& GetPosition() const { return position; }

	// 描画サイズ設定
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	// 描画サイズ取得
	const DirectX::XMFLOAT2& GetSize() const { return size; }

	// スケール設定
	void SetScale(DirectX::XMFLOAT2 scale) { this->scale = scale; }
	
	// スケール取得
	const DirectX::XMFLOAT2& GetScale() const { return scale; }

	// 角度設定
	void SetAngle(float angle) { this->angle = angle; }

	// 角度取得
	const float& GetAngle() const { return angle; }

	// テクスチャ設定
	void SetTexture(const char* filename);

	// テクスチャ取得
	Texture* GetTexture() const { return texture.get(); }
private:
	std::string					name;
	std::shared_ptr<Texture>	texture;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	DirectX::XMFLOAT2			scale;
	float						angle;
	std::shared_ptr<Charactor>	charactor;
};