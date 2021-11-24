#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Enemy.h"
#include "GameDatabase.h"

// �O���錾
class Actor;

// �G�𐶐�������֐�
//class CreateEnemy
//{
//public:
//	CreateEnemy();
//	~CreateEnemy();
//
//	// �G�𐶐�
//	void CreateEnemies();
//
//	// �X���C���̃R���|�[�l���g��ǉ����鏈��
//	void AddComponentSlime(std::shared_ptr<Actor> actor);
//	// �X���C���̃R���|�[�l���g��ǉ����鏈��
//	void AddComponentMutant(std::shared_ptr<Actor> actor);
//};

// �G�̐������Ǘ�����N���X
//class PopEnemyController
//{
//public:
//	PopEnemyController();
//	~PopEnemyController();
//private:
//	CreateEnemy pop_enemy;
//};

class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();
public:
	// �C���X�^���X�擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// �G�l�~�[�o�^
	void EnemyRegister(std::shared_ptr<Enemy> enemy);

	// �G�l�~�[�폜
	void EnemyRemove(std::shared_ptr<Enemy> enemy);

	// �G�𐶐�
	void CreateEnemies(int i, std::string index_string);

	// �Q�[���f�[�^���瓾���o���ʒu�����_�ɏo���ʒu�������_���Z�o
	void GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin);

	// �S�Ă̓G��j��
	void AllRemove();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �G�̃R���|�[�l���g��ǉ����鏈��
	void AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> data);

	// �G���擾
	std::shared_ptr<Enemy> GetEnemy(int index) { return enemies.at(index); }
	// ID�œG���擾
	std::shared_ptr<Enemy> GetEnemyFromID(int id);

	// �C���f���g���擾
	const int& GetIndentity() const { return identity; }

	// �G�̃J�E���g�擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
public:
	// ��x�ɏo��������G�̏���Ɖ���
	enum EnemyCountLimit
	{
		Min_Limit = 2,
		Max_Limit = 10,
	};
private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	int identity;
	float appearance_range = 50.0f;
};