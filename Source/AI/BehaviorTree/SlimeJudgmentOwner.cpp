#include "SlimeJudgmentOwner.h"
#include "Enemy.h"
#include "Charactor.h"

//-----------------------------------------
// ”»’è
//-----------------------------------------
bool HeavyBodyAttackJudgment::Judgment()
{
	// hp‚ª”¼•ªˆÈ‰º‚ÌŽžskill”­“®‰Â”\
	int health = owner->GetCharactor()->GetHealth();
	int base_health = static_cast<int>(owner->GetCharactor()->GetMaxHealth() * 0.8f);
	if (health > base_health)
	{
		// SkillNode‚Ö‘JˆÚ‚Å‚«‚é
		return true;
	}
	return false;
}
