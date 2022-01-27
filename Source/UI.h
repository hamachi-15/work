#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include "Component.h"

class UINodeTree;
class Charactor;

//******************************
// 
// UIクラス
// 
//******************************
class UI
{
public:
	// コンストラクタ
	UI() {}

	// デストラクタ
	virtual ~UI();

	// 名前取得
	virtual std::string GetName() const = 0;

	// 開始処理
	virtual void Start() = 0;

	// 更新処理
	void Update(float elapsed_time);
	
	// 更新処理
	void Render(ID3D11DeviceContext* context);

	// ワールド座標からスクリーン座標に変換する
	DirectX::XMFLOAT2 ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
		const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
public:
	UINodeTree* ui_node_tree = nullptr;
};