#pragma once
#include <string>
#include <d3d11.h>

class UIActionBase;
class UINodeBase;
class UIData;

class UINodeTree
{
public:
	// コンストラクタ
	UINodeTree() {}
	// デストラクタ
	~UINodeTree();

	// UIノード登録
	void AddNode(std::string entry_name, std::string search_name, UIData* ui_data, UIActionBase* ui_action);

	// 実行
	void Run(float elapsed_time);

	// 描画処理
	void Render(ID3D11DeviceContext* context);

private:
	// ノード全削除
	void NodeAllClear(UINodeBase* del_node);
private:
	UINodeBase* root;
};