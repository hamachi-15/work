#pragma once
#include <DirectXMath.h>

enum	class	EnemyTerritoryTag;

enum class MessageType : int
{
	Message_Call_Help,				// ���Ԃ��Ă�(Enemy�A���^AI�̗����Ŏg�p)
	Message_Change_Attack_Right,	// �U�����̈ڏ�(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
	Message_Give_Attack_Right,		// �U������^����i���^AI����Enemy�ւ̃��b�Z�[�W�p�j
	Message_Ask_Attack_Right,		// �U������v��(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
	Message_Hit_Boddy,				// �̃R���W�������m����������(Enemy,Player�Ŏg�p)
	Message_Hit_Attack,				// �U������������(Enemy,Player�Ŏg�p)
	Message_GetHit_Attack			// �U�����󂯂�(Enemy,Player�Ŏg�p)
};

struct Message
{
	MessageType			message;	// ���b�Z�[�WID
	DirectX::XMFLOAT3	hit_position = {0.0f, 0.0f, 0.0f}; // 
	EnemyTerritoryTag territory_tag;
};

class Telegram
{	
public:
	// �R���X�g���N�^
	Telegram(int sender, int receiver, Message message);
		
	int				sender;				// ������ID
	int				receiver;			// �󂯎��ID
	Message			message_box;
};