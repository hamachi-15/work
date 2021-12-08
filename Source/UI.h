#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include "DirectXMath.h"
#include <d3d11.h>

class Texture;
class UI;

struct UIParameter
{
	const char*					filename;
	std::string					name;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	float						angle;
	std::shared_ptr<UI>			parent = nullptr;
};


class UI
{
public:
	// コンストラクタ
	UI(const UIParameter& parameter);

	// デストラクタ
	virtual ~UI();
	
	//
	void SetParameter(const UIParameter& parameter);

	// 名前設定
	void SetName(std::string name) { this->name = name; }

	// 名前取得
	const char* GetName() const { return name.c_str(); }

	// 更新処理
	virtual void Update(float elapsed_time) = 0;

	// 描画座標設定
	void SetPosition(DirectX::XMFLOAT2 position) { this->position = position; }

	// 描画座標取得
	const DirectX::XMFLOAT2& GetPosition() const { return position; }

	// 描画サイズ設定
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	// 描画サイズ取得
	const DirectX::XMFLOAT2& GetSize() const { return size; }

	// 角度設定
	void SetAngle(float angle) { this->angle = angle; }

	// 角度取得
	const float& GetAngle() const { return angle; }

	// テクスチャ設定
	void SetTexture(const char* filename);

	// テクスチャ取得
	Texture* GetTexture() const { return texture.get(); }

	// 親UI設定
	std::shared_ptr<UI> SetParent(std::shared_ptr<UI> parent) { return this->parent = parent; }

	// 親UI取得
	std::shared_ptr<UI> GetParent() const { return this->parent; }

	// ワールド座標からスクリーン座標に変換する
	DirectX::XMFLOAT2 ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
		const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:
	std::string					name;
	std::shared_ptr<Texture>	texture;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	float						angle;
	std::shared_ptr<UI>			parent = nullptr; // 親のUI
};

class UIManager
{
private:
	// コンストラクタ
	UIManager();
	// デストラクタ
	~UIManager();
public:
	// インスタンス取得
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Draw(ID3D11DeviceContext* context);

	// UI登録
	void RegisterUI(std::shared_ptr<UI> ui);

	// UI削除
	void UnRegisterUI(std::shared_ptr<UI> ui);
private:
	std::vector<std::shared_ptr<UI>> ui_list;
	std::vector<std::shared_ptr<UI>> remove;
};