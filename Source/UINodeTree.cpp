#include "UINodeTree.h"
#include "UINodeBase.h"
//--------------------------
// デストラクタ
//--------------------------
UINodeTree::~UINodeTree()
{
	NodeAllClear(root);
}

//--------------------------
// UIノード登録
//--------------------------
void UINodeTree::AddNode(std::string search_name, std::string entry_name, UIData* ui_data, UIActionBase* ui_action)
{
	// サーチする名前がなければ
	if (search_name == "")
	{
		// そのままノードを追加
		if (root == NULL)
		{
			root = new UINodeBase(entry_name, NULL, NULL, ui_data, ui_action, 1);
		}
	}
	else
	{
		// ノードを検索
		UINodeBase* search_node = root->SearchNode(search_name);
		// 検索した結果見つかれば
		if (search_node != NULL)
		{
			// 兄弟になるノードを取得し
			UINodeBase* sibling = search_node->GetLastChild();
			// 追加するノードを作成し
			UINodeBase* add_node = new UINodeBase(entry_name, search_node, sibling, ui_data, ui_action, search_node->GetHiererchyNo() + 1);
			// 検索にヒットしたノードの子ノードに追加
			search_node->AddChild(add_node);
		}
	}

}

//--------------------------
// 実行
//--------------------------
void UINodeTree::Run(float elapsed_time)
{
	// 実行処理
	root->Run(elapsed_time);
}

// 描画処理
void UINodeTree::Render(ID3D11DeviceContext* context)
{
	root->Render(context);
}

//--------------------------
// ノード全削除
//--------------------------
void UINodeTree::NodeAllClear(UINodeBase* del_node)
{
	size_t count = del_node->GetChildren().size();
	if (count > 0)
	{
		for (UINodeBase* node : del_node->GetChildren())
		{
			NodeAllClear(node);
		}
		delete del_node;
	}
	else
	{
		delete del_node;
	}
}
