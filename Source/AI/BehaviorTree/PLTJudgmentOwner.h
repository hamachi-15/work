#pragma once
#include "JudgmentBase.h"

// BlowJudgment�ɑJ�ڏo���邩
class BlowJudgment : public JudgmentBase
{
public:
	BlowJudgment(Enemy* enemy) : JudgmentBase(enemy){}
	// ����
	bool Judgment();
};