#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "Charactor.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ImGuiRenderer.h"

#include "Model.h"
//-----------------------------------------
// デストラクタ
//-----------------------------------------
BehaviorTree::~BehaviorTree()
{
	NodeAllClear(root);
}

//-----------------------------------------
// ノードのGUI描画
//-----------------------------------------
void BehaviorTree::DrawNodeGUI()
{
	DrawNodeGUI(root);
}

void BehaviorTree::DrawNodeGUI(NodeBase* node)
{
	// 矢印をクリック、またはノードをダブルクリックで階層を開く
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	
	// 選択フラグ
	if (select_root == node)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	// 子がいない場合は矢印をつけない
	size_t child_count = node->children.size();
	if (child_count <= 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	// ツリーノードを表示
	bool opened = ImGui::TreeNodeEx(node, nodeFlags, node->GetName().c_str());

	// アクティブ化されたノードは選択する
	if (ImGui::IsItemClicked())
	{
		select_root = node;
	}

	// 開かれている場合、子階層も同じ処理を行う
	if (opened && child_count > 0)
	{
		for (NodeBase* child : node->children)
		{
			DrawNodeGUI(child);
		}
		ImGui::TreePop();
	}
}

//-----------------------------------------
// ノード追加
//-----------------------------------------
void BehaviorTree::AddNode(
	std::string search_name, 
	std::string entry_name,
	int priority, 
	SelectRule select_rule, 
	JudgmentBase* judgment, 
	ActionBase* action)
{
	if (search_name != "")
	{
		NodeBase* search_node = root->SearchNode(search_name);
		if (search_node != NULL)
		{
			NodeBase* sibling = search_node->GetLastChild();
			NodeBase* add_node = new NodeBase(entry_name, search_node, sibling, priority, select_rule, judgment, action, search_node->GetHiererchyNo() + 1);
		
			search_node->AddChild(add_node);
		}
	}
	else {
		if (root == NULL)
		{
			root = new NodeBase(entry_name, NULL, NULL, priority, select_rule, judgment, action, 1);
		}
	}

}

//-----------------------------------------
// 推論
//-----------------------------------------
NodeBase* BehaviorTree::ActiveNodeInference(Enemy* enemy, BehaviorData* data)
{
	data->Init();
	return root->Inference(enemy, data);
 }

//-----------------------------------------
// シーケンスノードからの推論開始
//-----------------------------------------
NodeBase* BehaviorTree::SequenceBack(NodeBase* sequence_node, Enemy* enemy, BehaviorData* data)
{
	return sequence_node->Inference(enemy, data);
}

//-----------------------------------------
// 実行前処理
//-----------------------------------------
void BehaviorTree::Start(NodeBase* action_node)
{
	// ノード実行の準備
	action_node->Start();
}

//-----------------------------------------
// ノード実行
//-----------------------------------------
NodeBase* BehaviorTree::Run(Enemy* enemy, NodeBase* action_node, BehaviorData* data, float elapsed_time)
{
	// ノード実行
	ActionBase::State state = action_node->Run(elapsed_time);

	// TECK リアクション用
	if (enemy->GetCharactor()->GetDeathFlag())
	{
		if (action_node->GetName() != "Death")
		{
			return NULL;
		}
	}
	// 正常終了
	if (state == ActionBase::State::Complete)
	{
		// シーケンスの途中かを判断
		NodeBase* sequenceNode = data->PopSequenceNode();

		// 途中じゃないなら終了
		if (sequenceNode == NULL)
		{
			return NULL;
		}
		else {
			// 途中ならそこから始める
			return SequenceBack(sequenceNode, enemy, data);
		}
		// 失敗は終了
	}
	else if (state == ActionBase::State::Failed) {
		return NULL;
	}

	// 現状維持
	return action_node;
}

//-----------------------------------------
// ノード全削除
//-----------------------------------------
void BehaviorTree::NodeAllClear(NodeBase* del_node)
{
	size_t count = del_node->children.size();
	if (count > 0)
	{
		for (NodeBase* node : del_node->children)
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
