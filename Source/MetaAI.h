#pragma once
#include <memory>
#include <set>
#include "Telegram.h"

class Actor;
class Enemy;
class Player;
class Charactor;

class MetaAI
{
public:
	enum class Identity : int
	{
		Meta = 0,
		Collision = 1,	// �R���W������ID��1
		Player = 2,		// �v���C���[ID��2~4
		Enemy = 5		// �G�l�~�[ID��5�ȍ~��t�^
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
	
	// ���̃��\�b�h��telegram�N���X���쐬����
	// Enemy���p������N���X�̃��b�Z�[�W���V�[�o�[�̏������Ăяo���B
	//void Discharge(Enemy* receiver, const Telegram& telegram);
	void Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram);
	void Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram);

	// MetaAI�p�̃��V�[�u�������w��
	void Discharge(const Telegram& telegram);

	// ���b�Z�[�W��M�����Ƃ��̏���
	bool OnMessages(const Telegram& message);

	// ���b�Z�[�W���M�֐�
	void SendMessaging(int sender, int receiver, Message message, int enemy_identity = 0);

	// �G�̏o������
	void AppearanceEnemy(float elapsed_time);
private:
	std::set<Telegram>			priority;
	std::shared_ptr<Player>		player;

	float						pop_interval;
	int index;
};