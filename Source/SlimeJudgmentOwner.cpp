#include "SlimeJudgmentOwner.h"
#include "Enemy.h"
#include "Charactor.h"

//-----------------------------------------
// ����
//-----------------------------------------
bool HeavyBodyAttackJudgment::Judgment()
{
	// hp�������ȉ��̎�skill�����\
	int health = owner->GetCharactor()->GetHealth();
	int base_health = static_cast<int>(owner->GetCharactor()->GetMaxHealth() * 0.8f);
	if (health > base_health)
	{
		// SkillNode�֑J�ڂł���
		return true;
	}
	return false;
}
