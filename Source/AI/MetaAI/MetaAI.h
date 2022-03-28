#pragma once
#include <memory>
#include <set>
#include "Telegram.h"

class Actor;
class Enemy;
class Player;
class Charactor;
class Scene;

class MetaAI
{
public:
	enum class Identity : int
	{
		Meta = 0,
		WorldMap = 1,	// ���[���h�}�b�v��ID
		BattleMap = 2,	// ���[���h�}�b�v��ID
		Collision = 3,	// �R���W������ID��1
		Player = 4,		// �v���C���[ID��2~4
		Enemy = 5,		// �G�l�~�[ID��5�ȍ~��t�^
		Slime,
		PLT,
		LAT,
		NightmearDragon,
		SoulEaterDragon
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	MetaAI();
	~MetaAI(){}

	// �C���X�^���X�擾
	static MetaAI& Instance()
	{
		static MetaAI instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);
	
	// Enemy�p�̃��V�[�u�������w��
	void Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram);

	// Player�p�̃��V�[�u�������w��
	void Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram);

	// MetaAI�p�̃��V�[�u�������w��
	void Discharge(const Telegram& telegram);

	//�V�[���p�̃��V�[�u�������w��
	void Discharge(Scene* receiver, const Telegram& telegram);

	// ���b�Z�[�W��M�����Ƃ��̏���
	bool OnMessages(const Telegram& message);

	// ���b�Z�[�W���M�֐�
	void SendMessaging(int sender, int receiver, Message message, int enemy_identity = 0);

	// �G�̏o������
	void AppearanceEnemy();
private:
	std::set<Telegram>			priority;
};