#pragma once
#include <memory>
#include <vector>

class Actor;

//*************************************************
// 
// �X�e�[�W�I�u�W�F�N�g�̃}�l�[�W���[�N���X
// 
//*************************************************
class StageManager
{
private:
	// �R���X�g���N�^
	StageManager() {}

	// �f�X�g���N�^
	~StageManager() {}

public:
	// �C���X�^���X�擾
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// �X�e�[�W�I�u�W�F�N�g�o�^����
	void Register();

	// �X�e�[�W�I�u�W�F�N�g�o�^��������
	void Unregister(std::shared_ptr<Actor> object);

	// �j������
	void Destroy();

	// �X�V����
	void Update(float elapsed_time);

private:
	std::vector<std::shared_ptr<Actor>> stage_objects;
};