#pragma once
#include <stack>
#include <map>
#include <vector>
#include "BehaviorTree.h"

class NodeBase;
class Enemy;

//**************************************
// 
// Behavior保存データクラス
// 
//**************************************
class BehaviorData
{
public:
	// コンストラクタ
	BehaviorData() { Init(); }

	// 初期化
	void Init();
	
	// 使用済みノードのリセット
	void ResetNodeUsed(std::vector<NodeBase*>* reset_hierachy);

	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase* node) { sequence_stack.push(node); }
	
	// シーケンスノードのポップ
	NodeBase* PopSequenceNode();

	// ノード使用判定
	bool IsNodeUsed(std::string name);

	// 使用済みノードに登録
	void EntryUsedNode(std::string name);

	// シーケンスステップのゲッター
	int GetSequenceStep(std::string name);
	
	// シーケンスステップのセッター
	void SetSequenceStep(std::string name, int step);
private:
	std::stack<NodeBase*> sequence_stack;				// シーケンスノードスタック
	std::map<std::string, int> run_sequence_step_map;		// 実行シーケンスのステップマップ
	std::map<std::string, bool> used_node_map;			// ノードの使用判定マップ
};
