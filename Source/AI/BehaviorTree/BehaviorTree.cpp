#include "BehaviorTree.h"
#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "Charactor.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ImGuiRenderer.h"

#include "Model.h"
//-----------------------------------------
// �f�X�g���N�^
//-----------------------------------------
BehaviorTree::~BehaviorTree()
{
	NodeAllClear(root);
}

//-----------------------------------------
// �m�[�h��GUI�`��
//-----------------------------------------
void BehaviorTree::DrawNodeGUI()
{
	DrawNodeGUI(root);
}

void BehaviorTree::DrawNodeGUI(NodeBase* node)
{
	// �����N���b�N�A�܂��̓m�[�h���_�u���N���b�N�ŊK�w���J��
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	
	// �I���t���O
	if (select_root == node)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	// �q�����Ȃ��ꍇ�͖������Ȃ�
	size_t child_count = node->children.size();
	if (child_count <= 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	// �c���[�m�[�h��\��
	bool opened = ImGui::TreeNodeEx(node, nodeFlags, node->GetName().c_str());

	// �A�N�e�B�u�����ꂽ�m�[�h�͑I������
	if (ImGui::IsItemClicked())
	{
		select_root = node;
	}

	// �J����Ă���ꍇ�A�q�K�w�������������s��
	if (opened && child_count > 0)
	{
		for (NodeBase* child : node->children)
		{
			DrawNodeGUI(child);
		}
		ImGui::TreePop();
	}
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
			NodeBase* add_node = new NodeBase(entry_name, search_node, sibling, priority, select_rule, judgment, action, search_node->GetHiererchyNo() + 1);
		
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
	if (enemy->GetCharactor()->GetDeathFlag())
	{
		if (action_node->GetName() != "Death")
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
