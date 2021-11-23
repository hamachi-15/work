#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "Charactor.h"
#include "Enemy.h"
#include "BehaviorData.h"

//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
BehaviorTree::~BehaviorTree()
{
	NodeAllClear(root);
}

//-----------------------------------------
// �m�[�h�ǉ�
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
			NodeBase* add_node = new NodeBase(entry_name, search_node, sibling, priority, select_rule, judgment, action, search_node->GetHirerchyNo() + 1);
		
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
// ���_
//-----------------------------------------
NodeBase* BehaviorTree::ActiveNodeInference(Enemy* enemy, BehaviorData* data)
{
	data->Init();
	return root->Inference(enemy, data);
 }

//-----------------------------------------
// �V�[�P���X�m�[�h����̐��_�J�n
//-----------------------------------------
NodeBase* BehaviorTree::SequenceBack(NodeBase* sequence_node, Enemy* enemy, BehaviorData* data)
{
	return sequence_node->Inference(enemy, data);
}

//-----------------------------------------
// ���s�O����
//-----------------------------------------
void BehaviorTree::Start(NodeBase* action_node)
{
	// �m�[�h���s�̏���
	action_node->Start();
}

//-----------------------------------------
// �m�[�h���s
//-----------------------------------------
NodeBase* BehaviorTree::Run(Enemy* enemy, NodeBase* action_node, BehaviorData* data, float elapsed_time)
{
	// �m�[�h���s
	ActionBase::State state = action_node->Run(elapsed_time);

	// TECK ���A�N�V�����p
	if (enemy->GetCharactor()->GetDamageFlag() || enemy->GetCharactor()->GetDeathFlag())
	{
		if (action_node->GetName() != "Damage" && action_node->GetName() != "Death")
		{
			return NULL;
		}
	}
	// ����I��
	if (state == ActionBase::State::Complete)
	{
		// �V�[�P���X�̓r�����𔻒f
		NodeBase* sequenceNode = data->PopSequenceNode();

		// �r������Ȃ��Ȃ�I��
		if (sequenceNode == NULL)
		{
			return NULL;
		}
		else {
			// �r���Ȃ炻������n�߂�
			return SequenceBack(sequenceNode, enemy, data);
		}
		// ���s�͏I��
	}
	else if (state == ActionBase::State::Failed) {
		return NULL;
	}

	// ����ێ�
	return action_node;
}

//-----------------------------------------
// �m�[�h�S�폜
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
