#pragma once
#include <string>
#include <d3d11.h>
#include <memory>
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
	UI();

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

public:
	std::unique_ptr<UINodeTree> ui_node_tree = nullptr;
};