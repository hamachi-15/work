#pragma once
#include "JudgmentBase.h"

// BlowJudgment‚É‘JˆÚo—ˆ‚é‚©
class BlowJudgment : public JudgmentBase
{
public:
	BlowJudgment(Enemy* enemy) : JudgmentBase(enemy){}
	// ”»’è
	bool Judgment();
};