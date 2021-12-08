#pragma once
#include <string>

class ActionBase;
class JudgmentBase;
class NodeBase;

class BehaviorData;
class Enemy;

class BehaviorTree
{
public:
	// �I�����[��
	enum class SelectRule
	{
		Non,
		Priority,				// �D�揇��
		Sequence,				// �V�[�P���X
		SequentialLooping,		// �V�[�P���V�������[�s���O
		Random					// �����_��
	};
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	BehaviorTree() : root(NULL){}
	~BehaviorTree();

	// ���s�m�[�h�𐄘_����
	NodeBase* ActiveNodeInference(Enemy* enemy, BehaviorData* data);

	// �V�[�P���X�m�[�h���琄�_�J�n
	NodeBase* SequenceBack(NodeBase* sequence_node, Enemy* enemy, BehaviorData* data);

	// �m�[�h�ǉ�
	void AddNode(std::string search_name, std::string entry_name, int priority, SelectRule select_rule, JudgmentBase* judgment, ActionBase* action);

	// ���s�O����
	void Start(NodeBase* action_node);

	// ���s
	NodeBase* Run(Enemy* enemy, NodeBase* action_node, BehaviorData* data, float elapsed_time);

	// �m�[�h��GUI�`��
	void DrawNodeGUI();

private:
	// �m�[�h��GUI�`��
	void DrawNodeGUI(NodeBase* node);
	// �m�[�h�S�폜
	void NodeAllClear(NodeBase* del_node);
private:
	// ���[�g�m�[�h
	NodeBase* root = nullptr;
	NodeBase* select_root = nullptr;
};

