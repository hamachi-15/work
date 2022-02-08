#pragma once
#include "JudgmentBase.h"


//*****************************************
// 
// 爪攻撃ノードに移行できるか判定
// 
//*****************************************
class ClawAttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	ClawAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// 判定
	bool Judgment() override;
};

//*****************************************
// 
// 噛みつき攻撃ノードに移行できるか判定
// 
//*****************************************
class BasicAttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	BasicAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// 判定
	bool Judgment() override;
};

//*****************************************
// 
// 角攻撃ノードに移行できるか判定
// 
//*****************************************
class HornAttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	HornAttackJudgment(Enemy* enemy) :JudgmentBase(enemy) {}

	// 判定
	bool Judgment() override;
};

//*****************************************
// 
// ボディプレス攻撃ノードに移行できるか判定
// 
//*****************************************
class BodyPressAttackJudgment : public JudgmentBase
{
public:
	// コンストラクタ
	BodyPressAttackJudgment(Enemy* enemy) : JudgmentBase(enemy) {}

	// 判定
	bool Judgment() override;
};

//*****************************************
// 
// 軸合わせノードに移行できるか判定
// 
//*****************************************
class TurnToTargetJudgment :public JudgmentBase
{
public:
	// コンストラクタ
	TurnToTargetJudgment(Enemy* enemy) : JudgmentBase(enemy) {}

	//判定
	bool Judgment() override;
};