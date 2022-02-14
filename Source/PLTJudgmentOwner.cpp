#include "PLTJudgmentOwner.h"
#include "MetaAI.h"
#include "Enemy.h"

//-------------------------------------
// BlowJudgment�ɑJ�ڏo���邩����
//-------------------------------------
bool BlowJudgment::Judgment()
{
    // ���^AI�ɍU������v��
    Message message;
    message.message = MessageType::Message_Ask_Attack_Right;
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Enemy),   // ���M��
        static_cast<int>(MetaAI::Identity::Meta),          // ��M��
        message);                                        // ���b�Z�[�W
    // �U�����������
    if (owner->GetRightOfAttack())
    {
        return true;
    }

	return false;
}
