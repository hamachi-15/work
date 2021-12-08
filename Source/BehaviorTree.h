#pragma once
#include <string>

class ActionBase;
class JudgmentBase;
class NodeBase;

class BehaviorData;
class Enemy;

class BehaviorTree
{
public:
	// 選択ルール
	enum class SelectRule
	{
		Non,
		Priority,				// 優先順位
		Sequence,				// シーケンス
		SequentialLooping,		// シーケンシャルルーピング
		Random					// ランダム
	};
public:
	// コンストラクタ・デストラクタ
	BehaviorTree() : root(NULL){}
	~BehaviorTree();

	// 実行ノードを推論する
	NodeBase* ActiveNodeInference(Enemy* enemy, BehaviorData* data);

	// シーケンスノードから推論開始
	NodeBase* SequenceBack(NodeBase* sequence_node, Enemy* enemy, BehaviorData* data);

	// ノード追加
	void AddNode(std::string search_name, std::string entry_name, int priority, SelectRule select_rule, JudgmentBase* judgment, ActionBase* action);

	// 実行前処理
	void Start(NodeBase* action_node);

	// 実行
	NodeBase* Run(Enemy* enemy, NodeBase* action_node, BehaviorData* data, float elapsed_time);

	// ノードのGUI描画
	void DrawNodeGUI();

private:
	// ノードのGUI描画
	void DrawNodeGUI(NodeBase* node);
	// ノード全削除
	void NodeAllClear(NodeBase* del_node);
private:
	// ルートノード
	NodeBase* root = nullptr;
	NodeBase* select_root = nullptr;
};

