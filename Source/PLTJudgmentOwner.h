#pragma once
#include "JudgmentBase.h"

// BlowJudgmentに遷移出来るか
class BlowJudgment : public JudgmentBase
{
public:
	BlowJudgment(Enemy* enemy) : JudgmentBase(enemy){}
	// 判定
	bool Judgment();
};