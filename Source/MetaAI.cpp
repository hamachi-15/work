#include "MetaAI.h"
#include "ActorManager.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Charactor.h"

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
MetaAI::MetaAI()
{
	// �v���C���[�A�N�^�[�̃v���C���[�R���|�[�l���g���擾
	player = ActorManager::Instance().GetActor("Player")->GetComponent<Player>();
	
	// �ŏ�����G���o��������
	int count = 1;
	while (1)
	{
		// �������Ă���G�̑��������Ĉ��ȉ��ɂȂ�Ώo�������Ă���
		int enemy_count = EnemyManager::Instance().GetEnemyCount();
		if (count <= EnemyManager::EnemyCountLimit::Min_Limit)
		{
			AppearanceEnemy(0.0f);
			count++;
		}
		else //if (count >= 1/*EnemyManager::EnemyCountLimit::Min_Limit*/)
		{
			break;
		}
	}
}

//-----------------------------------
// �X�V����
//-----------------------------------
void MetaAI::Update(float elapsed_time)
{
	// �C���^�[�o�����I�����Ă����
	//if (pop_interval <= 0.0f)
	//{
	//	// �������Ă���G�̑��������Ĉ��ȉ��ɂȂ�Ώo�������Ă���
	//	int enemy_count = EnemyManager::Instance().GetEnemyCount();
	//	// �G�̐������������ȉ��Ȃ�G���o��������
	//	if (enemy_count <= EnemyManager::EnemyCountLimit::Max_Limit)
	//	{
	//		AppearanceEnemy(elapsed_time);
	//	}
	//}
	//else
	//{
	//	// �C���^�[�o��������������
	//	pop_interval -= elapsed_time;
	//}
	
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
		//��M�ł��Ȃ������Ƃ��̏���
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
		//��M�ł��Ȃ������Ƃ��̏���
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
		//��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
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
		int count = 0;//�S�Ă̓G�ɂ��āA�U�����������Ă��Ȃ������ׂ�B
		int enemy_count = EnemyManager::Instance().GetEnemyCount();
		for (int i = 0; i < enemy_count; ++i)
		{
			std::shared_ptr<Enemy> enemy = EnemyManager::Instance().GetEnemy(i);
			if (enemy->GetAttackFlag()) count++;
		}
		//���ɍU���������Ă���G�����Ȃ��Ȃ�A�v�����Ă����G�ɍU������^���郁�b�Z�[�W�𑗂�B
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
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}
	else if(receiver == static_cast<int>(MetaAI::Identity::Player))
	{// �v���C���[����M��
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, message);
		Discharge(player, telegram);
	}
	else // �v���C���[�ƃ��^�ȊO�͓G
	{// �G����M�҂̏ꍇ�ǂ̓G�ɑ��M���邩
		// ���V�[�o�[�̃|�C���^���擾
		std::shared_ptr<Enemy> receive_enemy = enemy_manager.GetEnemyFromID(receiver);
		// ���V�[�o�[�����Ȃ���ΏI��
		if (receive_enemy == nullptr) return;
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, message);
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_enemy, telegram);
	}
}

//-----------------------------------
// �G�̏o������
//-----------------------------------
void MetaAI::AppearanceEnemy(float elapsed_time)
{
	int enemy_data_index = rand() % 2;
	// �A�N�^�[�̌��ɂ���C���f�b�N�X�ԍ���string�^�ɕϊ�����
	std::string index_string = std::to_string(index);
	// �G�𐶐�
	EnemyManager::Instance().CreateEnemies(enemy_data_index, index_string);
	// �C���^�[�o���̎��Ԃ�ݒ�
	pop_interval = 5.0f;
	// �C���f�b�N�X�ԍ��̃C���N�������g
	index++;
}