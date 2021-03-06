#include "SlimeJudgmentOwner.h"
#include "Enemy.h"
#include "Charactor.h"

//-----------------------------------------
// 判定
//-----------------------------------------
bool HeavyBodyAttackJudgment::Judgment()
{
	// hpが半分以下の時skill発動可能
	int health = owner->GetCharactor()->GetHealth();
	int base_health = static_cast<int>(owner->GetCharactor()->GetMaxHealth() * 0.8f);
	if (health > base_health)
	{
		// SkillNodeへ遷移できる
		return true;
	}
	return false;
}
