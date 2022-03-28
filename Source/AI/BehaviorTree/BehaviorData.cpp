#include "BehaviorData.h"
#include "NodeBase.h"

//**************************************
// 
// Behavior保存データクラス
// 
//**************************************
//-------------------------------------
// 初期化
//-------------------------------------
void BehaviorData::Init()
{
    run_sequence_step_map.clear();
    while (sequence_stack.size() > 0)
    {
        sequence_stack.pop();
    }
}
//-------------------------------------
// 使用済みノードのリセット
//-------------------------------------
void BehaviorData::ResetNodeUsed(std::vector<NodeBase*>* reset_hierachy)
{
    for (auto itr = reset_hierachy->begin(); itr != reset_hierachy->end(); itr++)
    {
        used_node_map[(*itr)->GetName()] = false;
    }
}

//-------------------------------------
// シーケンスノードのポップ
//-------------------------------------
NodeBase* BehaviorData::PopSequenceNode()
{
    // 空ならNULL
    if (sequence_stack.empty() != 0)
    {
        return NULL;
    }
    NodeBase* node = sequence_stack.top();
    if (node != NULL)
    {
        // 取りだしたデータを削除
        sequence_stack.pop();
    }
    return node;
}

//--------------------------
// ノード使用判定
//--------------------------
bool BehaviorData::IsNodeUsed(std::string name)
{
    // 使用してなかったらfalse
    if (used_node_map.count(name) == 0)
    {
        return false;
    }

    return used_node_map[name];
}

//-------------------------------------
// 使用済みノードに登録
//-------------------------------------
void BehaviorData::EntryUsedNode(std::string name)
{
    used_node_map[name] = true;
}

//-------------------------------------
// シーケンスステップのゲッター
//-------------------------------------
int BehaviorData::GetSequenceStep(std::string name)
{
    if (run_sequence_step_map.count(name) == 0)
    {
        run_sequence_step_map.insert(std::make_pair(name,0));
    }
    return run_sequence_step_map.at(name);
}

//-------------------------------------
// シーケンスステップのセッター
//-------------------------------------
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    run_sequence_step_map.at(name) = step;
}

