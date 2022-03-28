#pragma once

class Enemy;

class JudgmentBase
{
public:
	// �R���X�g���N�^
	JudgmentBase(Enemy* enemy) : owner(enemy) {}
	
	// ���菈��
	virtual bool Judgment() = 0;
protected:
	Enemy* owner;
};