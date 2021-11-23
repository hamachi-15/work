#pragma once
#include "ActionBase.h"

// パンチ攻撃行動
class BlowAttackAction : public ActionBase
{
public:
	// コンストラクタ
	BlowAttackAction(Enemy* enemy) : ActionBase(enemy){}

	// パンチ攻撃行動前処理
	void Start(std::string action_name)override;
	
	// パンチ攻撃行動処理
	ActionBase::State Run(float elapsed_time) override;

};