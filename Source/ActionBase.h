#pragma once
#include <string>

class Enemy;

// �s���������N���X
class ActionBase
{
public:
	ActionBase(Enemy* enemy) :owner(enemy) {}
	// ���s���
	enum class State
	{
		Run,		// ���s��
		Failed,		// ���s���s
		Complete,	// ���s����
	};
	// ���s�O����(�������z�֐�)
	virtual void Start(std::string action_name) = 0;

	// ���s����(�������z�֐�)
	virtual ActionBase::State Run(float elapsed_time) = 0;
protected:
	Enemy* owner;
};
