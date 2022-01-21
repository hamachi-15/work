#include "BehaviorData.h"
#include "NodeBase.h"

//**************************************
// 
// Behavior�ۑ��f�[�^�N���X
// 
//**************************************
//-------------------------------------
// ������
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
// �g�p�ς݃m�[�h�̃��Z�b�g
//-------------------------------------
void BehaviorData::ResetNodeUsed(std::vector<NodeBase*>* reset_hierachy)
{
    for (auto itr = reset_hierachy->begin(); itr != reset_hierachy->end(); itr++)
    {
        used_node_map[(*itr)->GetName()] = false;
    }
}

//-------------------------------------
// �V�[�P���X�m�[�h�̃|�b�v
//-------------------------------------
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

//--------------------------
// �m�[�h�g�p����
//--------------------------
bool BehaviorData::IsNodeUsed(std::string name)
{
    // �g�p���ĂȂ�������false
    if (used_node_map.count(name) == 0)
    {
        return false;
    }

    return used_node_map[name];
}

//-------------------------------------
// �g�p�ς݃m�[�h�ɓo�^
//-------------------------------------
void BehaviorData::EntryUsedNode(std::string name)
{
    used_node_map[name] = true;
}

//-------------------------------------
// �V�[�P���X�X�e�b�v�̃Q�b�^�[
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
// �V�[�P���X�X�e�b�v�̃Z�b�^�[
//-------------------------------------
void BehaviorData::SetSequenceStep(std::string name, int step)
{
    run_sequence_step_map.at(name) = step;
}

