#include "PLTJudgmentOwner.h"
#include "MetaAI.h"
#include "Enemy.h"

//-------------------------------------
// BlowJudgmentに遷移出来るか判定
//-------------------------------------
bool BlowJudgment::Judgment()
{
    // メタAIに攻撃権を要求
    Message message;
    message.message = MessageType::Message_Ask_Attack_Right;
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Enemy),   // 送信元
        static_cast<int>(MetaAI::Identity::Meta),          // 受信先
        message);                                        // メッセージ
    // 攻撃権があれば
    if (owner->GetRightOfAttack())
    {
        return true;
    }

	return false;
}
