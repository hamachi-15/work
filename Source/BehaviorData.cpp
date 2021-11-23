#include "BehaviorData.h"
#include "NodeBase.h"

// �V�[�P���X�m�[�h�̃|�b�v
NodeBase* BehaviorData::PopSequenceNode()
{
    // ��Ȃ�NULL
    if (sequence_stack.empty() != 0)
    {
        return NULL;
    }
    NodeBase* node = sequence_stack.top();
    if (node != NULL)
    {
        // ��肾�����f�[�^���폜
        sequence_stack.pop();
    }
    return node;
}
// �V�[�P���X�X�e�b�v�̃Q�b�^�[
int BehaviorData::GetSequenceStep(std::string name)
{
    if (run_sequence_step_map.count(name) == 0)
    {
        run_sequence_step_map.insert(std::make_pair(name,0));
    }
    return run_sequence_step_map.at(name);
}
// �V�[�P���X�X�e�b�v�̃Z�b�^�[
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    run_sequence_step_map.at(name) = step;
}
// ������
void BehaviorData::Init()
{
    run_sequence_step_map.clear();
    while (sequence_stack.size() > 0)
    {
        sequence_stack.pop();
    }
}
