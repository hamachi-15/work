#include "NodeBase.h"
#include "JudgmentBase.h"
#include "NodeBase.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ActionBase.h"
#include "Mathf.h"
// TODO �R�����g
// �f�X�g���N�^
NodeBase::~NodeBase()
{
	delete judgment;
	delete action;
}

//--------------------------
// �q�m�[�h�擾
//--------------------------
NodeBase* NodeBase::GetChild(int index)
{
	if (children.size() > index)
		return NULL;
	
	return children.at(index);
}

//--------------------------
// �q�m�[�h�擾(����)
//--------------------------
NodeBase* NodeBase::GetLastChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(children.size() - 1);
}

//--------------------------
// �q�m�[�h�擾(�擪)
//--------------------------
NodeBase* NodeBase::GetTopChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(0);
}

//--------------------------
// ���s�ۏ���
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
// �D�揇�ʑI��
//--------------------------
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* select_node = NULL;
	int priority = INT_MAX;

	// �D�揇�ʂ̍����m�[�h��list����T��
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
// �����_��
//--------------------------
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int select_no = rand() % list->size();
	return (*list).at(select_no);
}

//--------------------------
// �I���E�I�t�I��
//--------------------------
NodeBase* NodeBase::SelectOnOff(std::vector<NodeBase*>* list, BehaviorData* data)
{
	// ����̌��ʂ������Ȃ���Ή������Ȃ�
	if (list->size() == 0) return nullptr;

	std::vector<NodeBase*> off_list;

	// �g�p�ς݃m�[�h���X�g�����X�g�A�b�v����
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// �g�p���Ă��Ȃ��m�[�h�Ȃ�
		if (data->IsNodeUsed((*itr)->GetName()) == false)
		{	// ���X�g�ɒǉ�
			off_list.push_back(*itr);
		}
	}

	// ���X�g�A�b�v���������`�F�b�N
	if (off_list.size() == 0)
	{
		// �m�[�h��S�ă��Z�b�g
		data->ResetNodeUsed(&children);
		off_list = *list;
	}

	// �Z���N�g���[�����I���E�I�t�����_���̎�
	if (select_rule == BehaviorTree::SelectRule::On_Off_Ramdom)
	{
		int index = static_cast<int>(Mathf::RandomRange(0, static_cast<float>(off_list.size())));
		// �g�p�����m�[�h��o�^
		data->EntryUsedNode(off_list[index]->GetName());

		// �����_���Ɍ��߂��C���f�b�N�X�̃m�[�h������m�[�h
		return off_list[index];
	}
	else
	{
		// �g�p�����m�[�h��o�^
		data->EntryUsedNode(off_list[0]->GetName());

		// ���X�g�̐擪������m�[�h
		return off_list[0];
	}
}

//--------------------------
// �V�[�P���X�I��
//--------------------------
NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	int step = data->GetSequenceStep(name);
	// ���݂̃X�e�b�v���q�m�[�h�̃T�C�Y���傫�����
	if (step >= children.size())
	{
		// �V�[�P���X���[�v�ɐݒ肳��Ă��Ȃ����NULL��Ԃ�
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
// �m�[�h����
//--------------------------
NodeBase* NodeBase::SearchNode(std::string search_name)
{
	// ���O����v
	if (name == search_name)
	{
		return this;
	}
	else {
		// �q�m�[�h�Ō���
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
// �m�[�h���_
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
			// ����N���X���Ȃ���Ζ������ɒǉ�
			list.push_back(children.at(i));
		}
	}
	// �I�����[���Ńm�[�h����
	switch (select_rule)
	{
		// �D�揇��
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// �����_��
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// �I���E�I�t
	case BehaviorTree::SelectRule::On_Off:
	case BehaviorTree::SelectRule::On_Off_Ramdom:
		result = SelectOnOff(&list, data);
		break;
		// �V�[�P���X
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::Sequential_Looping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != NULL)
	{
		// �s��������ΏI��
		if (result->HasAction() == true)
		{
			return result;
		}
		else {
			// ���܂����m�[�h�Ő��_�J�n
			result = result->Inference(enemy, data);
		}
	}

	return result;
}

//--------------------------
// ���s�O����
//--------------------------
void NodeBase::Start()
{
	action->Start();
}

//--------------------------
// ���s����
//--------------------------
ActionBase::State NodeBase::Run(float elapsed_time)
{
	// �A�N�V�����m�[�h������Ύ��s���Č��ʂ�Ԃ��A�Ȃ���Ύ��s����
	if (action != NULL)
	{
		return action->Run(elapsed_time);
	}

	return ActionBase::State::Failed;
}
