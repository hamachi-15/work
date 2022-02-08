
#include "Mathf.h"

#include "MetaAI.h"

#include "SceneManager.h"

#include "Charactor.h"
#include "ActorManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
MetaAI::MetaAI()
{	
}

//-----------------------------------
// �X�V����
//-----------------------------------
void MetaAI::Update(float elapsed_time)
{
	// �v���C���[�����Ȃ��ɓG����萔�|���ΓG�̃X�e�[�^�X������������
	// ��莞�ԓ��̃v���C���[�ւ̃_���[�W�񐔂�����񐔈ȏ�Ȃ�G�̃X�e�[�^�X���㉻������
}

//-----------------------------------
// �v���C���[�փ��b�Z�[�W���΂�
//-----------------------------------
void MetaAI::Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// �G�փ��b�Z�[�W���΂�
//-----------------------------------
void MetaAI::Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// MetaAI�p�̃��V�[�u�������w��
//-----------------------------------
void MetaAI::Discharge(const Telegram& telegram)
{
	if (!OnMessages(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// �V�[���֑���p�̃��V�[�u�������w��
//-----------------------------------
void MetaAI::Discharge(Scene* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Scene Receive Failed");
	}
}

//-----------------------------------
// ���b�Z�[�W��M�����Ƃ��̏���
//-----------------------------------
bool MetaAI::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Ask_Attack_Right:
	{
		int count = 0;// �S�Ă̓G�ɂ��āA�U�����������Ă��Ȃ������ׂ�B
		int enemy_count = EnemyManager::Instance().GetEnemyCount();
		for (int i = 0; i < enemy_count; ++i)
		{
			std::shared_ptr<Enemy> enemy = EnemyManager::Instance().GetEnemy(i);
			if (enemy->GetAttackFlag()) count++;
		}
		// ���ɍU���������Ă���G�����Ȃ��Ȃ�A�v�����Ă����G�ɍU������^���郁�b�Z�[�W�𑗂�B
		if (count < 1)
		{
			// �N���U�����������Ă��Ȃ����MetaAI���瑗�M�҂ɍU�����t�^�̃��b�Z�[�W�𑗂�
			Message message_box;
			message_box.message = MessageType::Message_Give_Attack_Right;
			SendMessaging(static_cast<int>(MetaAI::Identity::Meta), telegram.sender, message_box);
		}
	}
		break;
	case MessageType::Message_Hit_Attack:
		
		break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//-----------------------------------
// ���b�Z�[�W���M�֐�
//-----------------------------------
void MetaAI::SendMessaging(int sender, int receiver, Message message, int enemy_identity)
{
	EnemyManager& enemy_manager = EnemyManager::Instance();
	if (receiver == static_cast<int>(MetaAI::Identity::Meta))
	{// MetaAI���Ă̎�
		Telegram telegram(sender, receiver, message);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}
	else if(receiver == static_cast<int>(MetaAI::Identity::WorldMap))
	{// 
		Telegram telegram(sender, receiver, message);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(SceneManager::Instance().GetCurrentScene(), telegram);
	}
	else if (receiver == static_cast<int>(MetaAI::Identity::BattleMap))
	{
		Telegram telegram(sender, receiver, message);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(SceneManager::Instance().GetCurrentScene(), telegram);
	}
	else if(receiver == static_cast<int>(MetaAI::Identity::Player))
	{// �v���C���[����M��
		// �v���C���[�R���|�[�l���g���擾
		std::shared_ptr<Player> player = ActorManager::Instance().GetActor("Player")->GetComponent<Player>();
		// ���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, message);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(player, telegram);
	}
	else // �v���C���[�ƃ��^�ȊO�͓G
	{// �G����M�҂̏ꍇ�ǂ̓G�ɑ��M���邩
		// ���V�[�o�[�̃|�C���^���擾
		std::shared_ptr<Enemy> receive_enemy = enemy_manager.GetEnemyFromID(receiver);
		// ���V�[�o�[�����Ȃ���ΏI��
		if (receive_enemy == nullptr) return;
		// ���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, message);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_enemy, telegram);
	}
}

//-----------------------------------
// �G�̏o������
//-----------------------------------
void MetaAI::AppearanceEnemy()
{
	// �G�̓꒣��ݒ�
	EnemyManager::Instance().CreateTerritory();
	// �G�𐶐�
	EnemyManager::Instance().CreateEnemies();
}
