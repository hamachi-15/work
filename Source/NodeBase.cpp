#include "NodeBase.h"
#include "JudgmentBase.h"
#include "NodeBase.h"
#include "Enemy.h"
#include "BehaviorData.h"
#include "ActionBase.h"

NodeBase::~NodeBase()
{
	delete judgment;
	delete action;
}

NodeBase* NodeBase::GetChild(int index)
{
	if (children.size() > index)
		return NULL;
	
	return children.at(index);
}

NodeBase* NodeBase::GetLastChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(children.size() - 1);
}

NodeBase* NodeBase::GetTopChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(0);
}

bool NodeBase::Judgment()
{
	if (judgment != NULL)
	{
		return judgment->Judgment();
	}

	return true;
}

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

// �����_��
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	int select_no = rand() % list->size();
	return (*list).at(select_no);
}

NodeBase* NodeBase::SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data)
{
	int step = data->GetSequenceStep(name);

	if (step >= children.size())
	{
		if (select_rule != BehaviorTree::SelectRule::SequentialLooping)
		{
			return NULL;
		}
		else
		{
			step = 0;
		}
	}

	for(auto itr = list->begin(); itr != list->end(); itr++)
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
		// �V�[�P���X
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequentialLooping:
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
// ���s�O����
void NodeBase::Start()
{
	action->Start();
}

ActionBase::State NodeBase::Run(float elapsed_time)
{
	// �A�N�V�����m�[�h������Ύ��s���Č��ʂ�Ԃ��A�Ȃ���Ύ��s����
	if (action != NULL)
	{
		return action->Run(elapsed_time);
	}

	return ActionBase::State::Failed;
}
