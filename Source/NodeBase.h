#pragma once
#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"
// 前方宣言
class JudgmentBase;
class BehaviorData;

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// ノードベースクラス
class NodeBase
{
public:
	// コンストラクタ
	NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
		BehaviorTree::SelectRule select_rule, JudgmentBase* jugement, ActionBase* action, int hierarchy_no) :
		name(name), parent(parent), sibling(sibling), priority(priority),
		select_rule(select_rule), judgment(jugement), action(action), hierarchy_no(hierarchy_no),
		children(NULL) {}
	
	// デストラクタ
	~NodeBase();
	
	// 名前取得
	std::string GetName() { return name;}
	
	// 親ノード取得
	NodeBase* GetParent() { return parent; }
	
	// 子ノード取得
	NodeBase* GetChild(int index);
	
	// 子ノード取得(末尾)
	NodeBase* GetLastChild();
	
	// 子ノード取得(先頭)
	NodeBase* GetTopChild();
	
	// 兄弟ノード取得
	NodeBase* GetSibling() { return sibling; }
	
	// 階層番号取得
	int GetHirerchyNo() { return hierarchy_no; }
	
	// 優先順位取得
	int GetPriority() { return priority; }
	
	// 親ノード設定
	void SetParent(NodeBase* parent) { this->parent = parent; }
	
	// 子ノード追加
	void AddChild(NodeBase* child) { children.push_back(child); }
	
	// 兄弟ノード設定
	void SetSibling(NodeBase* sibling) { this->sibling = sibling; }
	
	// 行動データを持っているのか判定する処理
	bool HasAction() { return action != NULL ? true : false; }
		
	// 実行可否処理
	bool Judgment();
	
	// 優先順位選択
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	
	// ランダム選択
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	
	// オン・オフ選択
	NodeBase* SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data);
	
	// シーケンス選択
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
	
	// ノード検索
	NodeBase* SearchNode(std::string search_name);
	
	// ノード推論
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	
	// 実行前処理
	void Start();
	
	// 実行処理
	ActionBase::State Run(float elapsed_time);
	std::vector<NodeBase*>		children;		// 子ノード

protected:
	std::string					name;			// 名前取得
	BehaviorTree::SelectRule	select_rule;	// 選択ルール
	JudgmentBase*				judgment;		// 判定クラス
	ActionBase*					action;			// 実行クラス
	unsigned int				priority;		// 優先順位
	NodeBase*					parent;			// 親ノード
	NodeBase*					sibling;		// 兄弟ノード
	int							hierarchy_no;	// 階層番号
};

