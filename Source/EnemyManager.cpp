#include "EnemyManager.h"
#include "Mathf.h"
#include "Charactor.h"
#include "ActorManager.h"
#include "Movement.h"
#include "EnemySlime.h"
#include "EnemyMutant.h"
#include "EnemyPLT.h"
#include "EnemyLAT.h"
#include "EnemyDragonNightmare.h"
#include "Graphics.h"
#include "PhongVarianceShadowMap.h"
#include "LambertShader.h"
#include "Script.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
EnemyManager::EnemyManager()
{
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
EnemyManager::~EnemyManager()
{
}

//-----------------------------------------------
// �G��o�^
//-----------------------------------------------
void EnemyManager::EnemyRegister(std::shared_ptr<Enemy> enemy)
{
	// ID��ݒ�
	enemy->GetCharactor()->SetID(static_cast<int>(MetaAI::Identity::Enemy));
	enemy->SetIdentity(identity);
	//�ݒ肵����C���N�������g����
	identity++;		
	// �G���X�g�ɒǉ�
	enemies.emplace_back(enemy);
}

//-----------------------------------------------
// �G��j��
//-----------------------------------------------
void EnemyManager::EnemyRemove(std::shared_ptr<Enemy> enemy)
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = std::find(enemies.begin(), enemies.end(), enemy);
	enemies.erase(iterate_enemy);
}

//-----------------------------------------------
// �S�Ă̓G��j��
//-----------------------------------------------
void EnemyManager::AllRemove()
{
	std::vector<std::shared_ptr<Enemy>>::iterator iterate_enemy = enemies.begin();
	for (; iterate_enemy != enemies.end(); iterate_enemy = enemies.begin())
	{
		enemies.erase(iterate_enemy);
	}
	enemies.clear();
}

//-----------------------------------------------
// �f�o�b�O�v���~�e�B�u�`��
//-----------------------------------------------
void EnemyManager::DrawDebugPrimitive()
{
	for (std::shared_ptr<Enemy> enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

//-----------------------------------------------
// �G�𐶐�
//-----------------------------------------------
// TODO ���t�@�N�^�����O
void EnemyManager::CreateEnemies(int index, std::string index_string)
{
	std::vector<std::shared_ptr<EnemyOccurPosition>> enemy_appearance_data = GameDataBase::Instance().GetEnemyOccurData();
	std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyData();
	std::shared_ptr<EnemyData> enemy_data = data_list.at(index);

	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();

	std::shared_ptr<EnemyOccurPosition> appearance_data = enemy_appearance_data.at(index);
	DirectX::XMFLOAT3 appearance_position = { appearance_data->position_x,appearance_data->position_y, appearance_data->position_z };


	SetEnemyStatus(actor, enemy_data, index_string, appearance_position);
}

//-----------------------------------------------
// �X�N���v�g����G�𐶐�
//-----------------------------------------------
void EnemyManager::CreateEnemies(int id)
{
	std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyData();
	int index = 0;
	for (std::shared_ptr<EnemyData> data : data_list)
	{
		if (data->id == id)
		{
			std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
			std::vector<std::shared_ptr<EnemyData>> data_list = GameDataBase::Instance().GetEnemyData();
			std::shared_ptr<EnemyData> enemy_data = data_list.at(index);
			std::string index_string = std::to_string(index);
			SetEnemyStatus(actor, enemy_data, index_string, DirectX::XMFLOAT3{-126, 7, 169});
		}
		++index;
	}
}

//-----------------------------------------
// �X�N���v�g����G�����擾���ēG�𐶐�����
//-----------------------------------------
bool EnemyManager::CreateEnemyScriptData()
{
	Script* script = new Script("./Data/Script/SendBattleSceneScript.txt");
	char	strwork[256];

	while (1)
	{
		if (!script->SearchTop())
			break;
		if (script == nullptr)
		{
			return false;
		}
		//	��������t�@�C��������o��
		script->GetParamString(strwork);

		//	�I���R�}���h
		if (lstrcmpA(strwork, "END") == 0)
		{
			break;
		}

		if (lstrcmpA(strwork, "EnemyID") == 0)
		{
			// �X�N���v�g�̓GID����G�𐶐�
			CreateEnemies(script->GetParamInt());
		}
	}
	delete script;
	return true;
}

//-----------------------------------------
// �G�̃X�e�[�^�X��ݒ肷�鏈��
//-----------------------------------------
void EnemyManager::SetEnemyStatus(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> enemy_data, std::string index_string, DirectX::XMFLOAT3 appearance_position)
{
	// ���O�̐ݒ�
	std::string name = std::string(enemy_data->name) + index_string;
	actor->SetName(name.c_str());
	// �G�f�[�^ID�̐ݒ�
	actor->SetEnemyDataID(enemy_data->id);
	// ���f���̃Z�b�g�A�b�v
	actor->SetUpModel(enemy_data->model_path);
	// �o���ʒu�̐ݒ�
	GetAppearancePosition(actor, { appearance_position.x, appearance_position.y, appearance_position.z });
	// �X�P�[���̐ݒ�
	actor->SetScale({ enemy_data->scale_x, enemy_data->scale_y, enemy_data->scale_z });
	// �A���O���̐ݒ�
	actor->SetAngle({ enemy_data->angle_x, enemy_data->angle_y, enemy_data->angle_z });
	// �A�j���[�V�����m�[�h�̐ݒ�
	actor->SetAnimationNodeName(enemy_data->animation_node_name);
	// �V�F�[�_�[�̐ݒ�
	actor->AddShader<LambertShader>(Graphics::Instance().GetDevice());

	// �e�G�̃R���|�[�l���g�ǉ�
	AddComponent(actor, enemy_data);
	switch (enemy_data->category)
	{
	case EnemyCategory::Slime:
		actor->AddComponent<EnemySlime>();
		break;
	case EnemyCategory::LAT:
		actor->AddComponent<EnemyLAT>();
		break;
	case EnemyCategory::PLT:
		actor->AddComponent<EnemyPLT>();
		break;
	case EnemyCategory::Mutant:
		actor->AddComponent<EnemyMutant>();
		break;
	case EnemyCategory::NightmareDragon:
		actor->AddComponent<EnemyDragonNightmare>();
		break;
	case EnemyCategory::SoulEaterDragon:
		break;
	case EnemyCategory::DragonUsurper:
		break;
	}
}

//-----------------------------------------------
// �o���ʒu�����߂鏈��
//-----------------------------------------------
void EnemyManager::GetAppearancePosition(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 appearance_origin)
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, appearance_range);

	DirectX::XMFLOAT3 appearance_position;
	appearance_position.x = appearance_origin.x + sinf(theta) * range;
	appearance_position.y = appearance_origin.y;
	appearance_position.z = appearance_origin.z + cosf(theta) * range;
	actor->SetPosition(appearance_position);
}

//-----------------------------------------------
// �X���C���̃R���|�[�l���g��ǉ����鏈��
//-----------------------------------------------
void EnemyManager::AddComponent(std::shared_ptr<Actor> actor, std::shared_ptr<EnemyData> data)
{
	actor->AddComponent<Movement>();
	std::shared_ptr<Charactor> charactor = actor->AddComponent<Charactor>();
	charactor->SetMaxHealth(data->hp);
	charactor->SetHealth(data->hp);
}

//-----------------------------------------------
// ID�œG���擾
//-----------------------------------------------
std::shared_ptr<Enemy> EnemyManager::GetEnemyFromID(int id)
{
	int enemy_count = GetEnemyCount();
	for (int i = 0; i < enemy_count; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemies.at(i);
		// id�ƃA�C�f���e�B�e�B�������̓G���擾����
		if (enemy->GetCharactor()->GetID() + enemy->GetIdentity() ==  id)
		{
			return enemy;
		}
	}
	return nullptr;
}
