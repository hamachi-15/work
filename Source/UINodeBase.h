#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>

class Texture;
class UIActionBase;
class UI;
class UIData;

//*********************************
// 
// UIノードベースクラス
// 
//*********************************
class UINodeBase
{
public:
	// コンストラクタ
	UINodeBase(std::string name, UINodeBase* parent, UINodeBase* sibling, UIData* ui_data, UIActionBase* action, int hierarchy_no) :
		name(name), parent(parent), sibling(sibling), ui_data(ui_data), action(action), hierarchy_no(hierarchy_no) {}
	
	// 実行処理
	void Run(float elapsed_time);

	//描画処理
	void Render(ID3D11DeviceContext* context);

	void Rebder(ID3D11DeviceContext* context, Texture* texture, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size, const float& angle);

	// UIデータ検索
	UINodeBase* SearchNode(std::string name);

	// 親UI設定
	UINodeBase* SetParent(UINodeBase* parent) { return this->parent = parent; }

	// 親UI取得
	UINodeBase* GetParent() const { return this->parent; }

	// 子ノード追加
	void AddChild(UINodeBase* child) { children.push_back(child); }

	// 子ノード取得
	const std::vector<UINodeBase*>& GetChildren() { return children; }

	// 子ノード取得
	UINodeBase* GetChild(int index);

	// 子ノード取得(末尾)
	UINodeBase* GetLastChild();

	// 子ノード取得(先頭)
	UINodeBase* GetTopChild();

	// 兄弟ノード設定
	void SetSibling(UINodeBase* sibling) { this->sibling = sibling; }

	// 兄弟ノード取得
	UINodeBase* SetSibling() { return sibling; }
	
	// 階層番号取得
	int GetHiererchyNo() const { return hierarchy_no; }
private:
	std::string					name;			// 名前取得
	UIActionBase*				action;			// 実行クラス
	UINodeBase*					parent;			// 親ノード
	UIData*						ui_data;
	std::vector<UINodeBase*>	children;		// 子ノード
	UINodeBase*					sibling;		// 兄弟ノード
	int							hierarchy_no;	// 階層番号
};
