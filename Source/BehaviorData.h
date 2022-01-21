#pragma once
#include <stack>
#include <map>
#include <vector>
#include "BehaviorTree.h"

class NodeBase;
class Enemy;

//**************************************
// 
// Behavior�ۑ��f�[�^�N���X
// 
//**************************************
class BehaviorData
{
public:
	// �R���X�g���N�^
	BehaviorData() { Init(); }

	// ������
	void Init();
	
	// �g�p�ς݃m�[�h�̃��Z�b�g
	void ResetNodeUsed(std::vector<NodeBase*>* reset_hierachy);

	// �V�[�P���X�m�[�h�̃v�b�V��
	void PushSequenceNode(NodeBase* node) { sequence_stack.push(node); }
	
	// �V�[�P���X�m�[�h�̃|�b�v
	NodeBase* PopSequenceNode();

	// �m�[�h�g�p����
	bool IsNodeUsed(std::string name);

	// �g�p�ς݃m�[�h�ɓo�^
	void EntryUsedNode(std::string name);

	// �V�[�P���X�X�e�b�v�̃Q�b�^�[
	int GetSequenceStep(std::string name);
	
	// �V�[�P���X�X�e�b�v�̃Z�b�^�[
	void SetSequenceStep(std::string name, int step);
private:
	std::stack<NodeBase*> sequence_stack;				// �V�[�P���X�m�[�h�X�^�b�N
	std::map<std::string, int> run_sequence_step_map;		// ���s�V�[�P���X�̃X�e�b�v�}�b�v
	std::map<std::string, bool> used_node_map;			// �m�[�h�̎g�p����}�b�v
};
