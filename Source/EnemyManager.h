#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Enemy.h"
#include "GameDatabase.h"

// �O���錾
class Actor;

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
	// �꒣��̐ݒ�
	void CreateTerritory();

	// �G�l�~�[�o�^
	void EnemyRegister(std::shared_ptr<Enemy> enemy);

	// �G�l�~�[�폜
	void EnemyRemove(std::shared_ptr<Enemy> enemy);

	// �G�𐶐�
	void CreateEnemies();
	
	void CreateEnemies(int id);
	
	// �X�N���v�g����G�����擾���ēG�𐶐�����
	bool CreateEnemyScriptData();
	
	// �G�̃X�e�[�^�X��ݒ�
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyAppearancePosition> appearance_data);
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, int& string_id, DirectX::XMFLOAT3& appearance_position);

	// �Q�[���f�[�^���瓾���o���ʒu�����_�ɏo���ʒu�������_���Z�o
	void GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin, float appearance_range);

	// �S�Ă̓G��j��
	void AllRemove();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �G�̃R���|�[�l���g��ǉ����鏈��
	void AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> data, EnemyTerritoryTag& tag);

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
		Min_Limit = 1,
		Max_Limit = 4,
	};
private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	int identity;
};