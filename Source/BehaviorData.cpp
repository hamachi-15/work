#include "BehaviorData.h"
#include "NodeBase.h"

// シーケンスノードのポップ
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
// シーケンスステップのゲッター
int BehaviorData::GetSequenceStep(std::string name)
{
    if (run_sequence_step_map.count(name) == 0)
    {
        run_sequence_step_map.insert(std::make_pair(name,0));
    }
    return run_sequence_step_map.at(name);
}
// シーケンスステップのセッター
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    run_sequence_step_map.at(name) = step;
}
// 初期化
void BehaviorData::Init()
{
    run_sequence_step_map.clear();
    while (sequence_stack.size() > 0)
    {
        sequence_stack.pop();
    }
}
