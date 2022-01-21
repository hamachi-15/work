#pragma once
#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"
// �O���錾
class JudgmentBase;
class BehaviorData;

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// �m�[�h�x�[�X�N���X
class NodeBase
{
public:
	// �R���X�g���N�^
	NodeBase(std::string name, NodeBase* parent, NodeBase* sibling, int priority,
		BehaviorTree::SelectRule select_rule, JudgmentBase* jugement, ActionBase* action, int hierarchy_no) :
		name(name), parent(parent), sibling(sibling), priority(priority),
		select_rule(select_rule), judgment(jugement), action(action), hierarchy_no(hierarchy_no),
		children(NULL) {}
	
	// �f�X�g���N�^
	~NodeBase();
	
	// ���O�擾
	std::string GetName() { return name;}
	
	// �e�m�[�h�擾
	NodeBase* GetParent() { return parent; }
	
	// �q�m�[�h�擾
	NodeBase* GetChild(int index);
	
	// �q�m�[�h�擾(����)
	NodeBase* GetLastChild();
	
	// �q�m�[�h�擾(�擪)
	NodeBase* GetTopChild();
	
	// �Z��m�[�h�擾
	NodeBase* GetSibling() { return sibling; }
	
	// �K�w�ԍ��擾
	int GetHirerchyNo() { return hierarchy_no; }
	
	// �D�揇�ʎ擾
	int GetPriority() { return priority; }
	
	// �e�m�[�h�ݒ�
	void SetParent(NodeBase* parent) { this->parent = parent; }
	
	// �q�m�[�h�ǉ�
	void AddChild(NodeBase* child) { children.push_back(child); }
	
	// �Z��m�[�h�ݒ�
	void SetSibling(NodeBase* sibling) { this->sibling = sibling; }
	
	// �s���f�[�^�������Ă���̂����肷�鏈��
	bool HasAction() { return action != NULL ? true : false; }
		
	// ���s�ۏ���
	bool Judgment();
	
	// �D�揇�ʑI��
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	
	// �����_���I��
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	
	// �I���E�I�t�I��
	NodeBase* SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data);
	
	// �V�[�P���X�I��
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);
	
	// �m�[�h����
	NodeBase* SearchNode(std::string search_name);
	
	// �m�[�h���_
	NodeBase* Inference(Enemy* enemy, BehaviorData* data);
	
	// ���s�O����
	void Start();
	
	// ���s����
	ActionBase::State Run(float elapsed_time);
	std::vector<NodeBase*>		children;		// �q�m�[�h

protected:
	std::string					name;			// ���O�擾
	BehaviorTree::SelectRule	select_rule;	// �I�����[��
	JudgmentBase*				judgment;		// ����N���X
	ActionBase*					action;			// ���s�N���X
	unsigned int				priority;		// �D�揇��
	NodeBase*					parent;			// �e�m�[�h
	NodeBase*					sibling;		// �Z��m�[�h
	int							hierarchy_no;	// �K�w�ԍ�
};

