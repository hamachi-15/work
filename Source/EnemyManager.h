#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Enemy.h"
#include "GameDatabase.h"
#include "SceneManager.h"
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
	
	// �G�f�[�^ID����G�𐶐�
	void CreateEnemies(int id, EnemyTerritoryTag territory_tag);
	
	// �G���J�E���g�f�[�^����G�����擾���ēG�𐶐�����
	bool CreateEnemyEncountData();
	
	// �G�̃X�e�[�^�X��ݒ�
	void SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::shared_ptr<EnemyTerritoryPosition> teritory_datam, DirectX::XMFLOAT3 appearance_position = {0.0f, 0.0f, 0.0f});

	// �Q�[���f�[�^���瓾���e���g���[�����_�ɏo���ʒu�������_���Z�o
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

	// �G�̑����擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// �e���g���[�̓G�����j����Ă��邩�̃R���e�i���擾
	std::map<EnemyTerritoryTag, bool> GetDefeatTeritory() { return defeat_territory; }

	bool GetDefeatTeritory(EnemyTerritoryTag tag) { return defeat_territory[tag]; }

	// �e���g���[�̓G�����j����Ă��邩�̃t���O��ݒ�
	void SetDefeatTeritory(EnemyTerritoryTag tag, bool flag) { defeat_territory[tag] = flag; }

	// �o�g���V�[��������
	bool IsBattleScene() const { return (SceneManager::Instance().GetCurrentScene()->GetName() == "SceneBattle"); }
private:
	std::vector<std::shared_ptr<Enemy>>		enemies;						// �G�̔z��
	std::map<EnemyTerritoryTag, bool>		defeat_territory;				// �e���g���[�̓G�����j����Ă��邩�̃R���e�i
	int										identity;
	bool									appearance_ramdam_flag = true;	// �����_���Ȉʒu�ɏo�������邩�̃t���O
};