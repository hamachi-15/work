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
	
	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase* node) { sequence_stack.push(node); }
	
	// シーケンスノードのポップ
	NodeBase* PopSequenceNode();
	
	// シーケンスステップのゲッター
	int GetSequenceStep(std::string name);
	
	// シーケンスステップのセッター
	void SetSequenceStep(std::string name, int step);
private:
	std::stack<NodeBase*> sequence_stack;				// シーケンスノードスタック
	std::map<std::string, int> run_sequence_step_map;		// 実行シーケンスのステップマップ
};
