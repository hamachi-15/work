#include "NodeBase.h"
#include "JudgmentBase.h"
#include "NodeBase.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ActionBase.h"
#include "Mathf.h"
// TODO コメント
// デストラクタ
NodeBase::~NodeBase()
{
	delete judgment;
	delete action;
}

//--------------------------
// 子ノード取得
//--------------------------
NodeBase* NodeBase::GetChild(int index)
{
	if (children.size() > index)
		return NULL;
	
	return children.at(index);
}

//--------------------------
// 子ノード取得(末尾)
//--------------------------
NodeBase* NodeBase::GetLastChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(children.size() - 1);
}

//--------------------------
// 子ノード取得(先頭)
//--------------------------
NodeBase* NodeBase::GetTopChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(0);
}

//--------------------------
// 実行可否処理
//--------------------------
bool NodeBase::Judgment()
{
	if (judgment != NULL)
	{
		return judgment->Judgment();
	}

	return true;
}

//--------------------------
// 優先順位選択
//--------------------------
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* select_node = NULL;
	int priority = INT_MAX;

	// 優先順位の高いノードをlistから探す
	for (auto itr = list->begin(); itr != list->end(); ++itr)
	{
		if (priority > (*itr)->GetPriority())
		{
			select_node = (*itr);
			priority = (*itr)->GetPriority();
		}
	}
	return select_node;
}

//--------------------------
// ランダム
//--------------------------
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int select_no = rand() % list->size();
	return (*list).at(select_no);
}

//--------------------------
// オン・オフ選択
//--------------------------
NodeBase* NodeBase::SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data)
{
	// 判定の結果が何もなければ何もしない
	if (list->size() == 0) return nullptr;

	std::vector<NodeBase*> off_list;

	// 使用済みノードリストをリストアップする
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// 使用していないノードなら
		if (data->IsNodeUsed((*itr)->GetName()) == false)
		{	// リストに追加
			off_list.push_back(*itr);
		}
	}

	// リストアップした数をチェック
	if (off_list.size() == 0)
	{
		// ノードを全てリセット
		data->ResetNodeUsed(&children);
		off_list = *list;
	}

	// セレクトルールがオン・オフランダムの時
	if (select_rule == BehaviorTree::SelectRule::On_Off_Ramdom)
	{
		int index = static_cast<int>(Mathf::RandomRange(0, static_cast<float>(off_list.size())));
		// 使用したノードを登録
		data->EntryUsedNode(off_list[index]->GetName());

		// ランダムに決めたインデックスのノードが決定ノード
		return off_list[index];
	}
	else
	{
		// 使用したノードを登録
		data->EntryUsedNode(off_list[0]->GetName());

		// リストの先頭が決定ノード
		return off_list[0];
	}
}

//--------------------------
// シーケンス選択
//--------------------------
NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	int step = data->GetSequenceStep(name);
	// 現在のステップが子ノードのサイズより大きければ
	if (step >= children.size())
	{
		// シーケンスループに設定されていなければNULLを返す
		if (select_rule != BehaviorTree::SelectRule::Sequential_Looping)
		{
			return NULL;
		}
		else
		{
			step = 0;
		}
	}

	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		if (children.at(step)->GetName() == (*itr)->GetName())
		{
			data->PushSequenceNode(this);
			data->SetSequenceStep(GetName(), step + 1);
			return children.at(step);
		}

	}
	return NULL;
}

//--------------------------
// ノード検索
//--------------------------
NodeBase* NodeBase::SearchNode(std::string search_name)
{
	// 名前が一致
	if (name == search_name)
	{
		return this;
	}
	else {
		// 子ノードで検索
		for (auto itr = children.begin(); itr != children.end(); itr++)
		{
			NodeBase* ret = (*itr)->SearchNode(search_name);

			if (ret != NULL)
			{
				return ret;
			}
		}
	}

	return NULL;
}

//--------------------------
// ノード推論
//--------------------------
NodeBase* NodeBase::Inference(Enemy* enemy, BehaviorData* data)
{
	std::vector<NodeBase*> list;
	NodeBase* result = NULL;

	//
	for (int i = 0; i < children.size(); ++i)
	{
		if (children.at(i)->judgment != NULL)
		{
			if (children.at(i)->judgment->Judgment() == true)
			{
				list.push_back(children.at(i));
			}
		}
		else {
			// 判定クラスがなければ無条件に追加
			list.push_back(children.at(i));
		}
	}
	// 選択ルールでノード決め
	switch (select_rule)
	{
		// 優先順位
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// ランダム
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// オン・オフ
	case BehaviorTree::SelectRule::On_Off:
	case BehaviorTree::SelectRule::On_Off_Ramdom:
		result = SelectOnOff(&list, data);
		break;
		// シーケンス
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::Sequential_Looping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != NULL)
	{
		// 行動があれば終了
		if (result->HasAction() == true)
		{
			return result;
		}
		else {
			// 決まったノードで推論開始
			result = result->Inference(enemy, data);
		}
	}

	return result;
}

//--------------------------
// 実行前処理
//--------------------------
void NodeBase::Start()
{
	action->Start();
}

//--------------------------
// 実行処理
//--------------------------
ActionBase::State NodeBase::Run(float elapsed_time)
{
	// アクションノードがあれば実行して結果を返す、なければ失敗扱い
	if (action != NULL)
	{
		return action->Run(elapsed_time);
	}

	return ActionBase::State::Failed;
}
