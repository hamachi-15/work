// �}�l�[�W���[�C���N���[�h
#include "StageManager.h"
#include "ActorManager.h"
#include "CollisionManager.h"

// �R���W�����C���N���[�h
#include "CullingCollision.h"

// �A�N�^�[�E�R���|�[�l���g�C���N���[�h
#include "Actor.h"
#include "Stage.h"
#include "StageObjectCollision.h"

// �v�Z�C���N���[�h
#include "Mathf.h"

// �f�[�^�x�[�X�C���N���[�h
#include "GameDatabase.h"
#include "StageObjectData.h"
#include "WorldMapObjectPositionData.h"
#include "ButtleMapObjectPositionData.h"
#include "ActorType.h"

//*************************************************
// 
// �X�e�[�W�I�u�W�F�N�g�̃}�l�[�W���[�N���X
// 
//*************************************************
//--------------------------------
// �X�e�[�W�I�u�W�F�N�g�o�^����
//--------------------------------
void StageManager::ButtleObjectRegister()
{
	// �f�[�^�x�[�X�C���X�^���X�擾
	GameDataBase& data_base = GameDataBase::Instance();
	// �X�e�[�W�I�u�W�F�N�g�f�[�^���擾
	std::vector<std::shared_ptr<StageObjectData>> stage_object_list = data_base.GetButtleMapObjectDataList();
	// �o�g���}�b�v�̃I�u�W�F�N�g�z�u�ʒu�f�[�^���擾
	std::vector<std::shared_ptr<ButtleMapObjectPositionData>> stage_object_position_list = data_base.GetButtleMapObjectPositionDataList();
	// �f�[�^�̃T�C�Y�擾
	int data_max_count = data_base.GetButtleMapObjectPositionDataCount();
	
	// �I�u�W�F�N�g��������
	for (int i = 0; i < data_max_count; ++i)
	{
		// �I�u�W�F�N�g�z�u�f�[�^���擾
		std::shared_ptr<ButtleMapObjectPositionData> object_position_data = stage_object_position_list.at(i);
		// �I�u�W�F�N�g�̔z�u�f�[�^����I�u�W�F�N�g�f�[�^���擾����
		std::shared_ptr<StageObjectData> object_data = data_base.GetButtleMapObjectData(object_position_data->object_category);
		// �A�N�^�[���쐬
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// ���f���ǂݍ���
		actor->SetUpModel(object_data->model_path, nullptr);
		// �p�x�ݒ�
		actor->SetAngle({ object_data->angle_x, object_data->angle_y, object_data->angle_z });
		// �X�P�[���ݒ�
		actor->SetScale({ object_data->scale_x, object_data->scale_y, object_data->scale_z });
		// ���O�ݒ�
		actor->SetName(object_position_data->name);
		// �ʒu�ݒ�
		actor->SetPosition({ object_position_data->position_x, object_position_data->position_y, object_position_data->position_z });
		// �R���|�[�l���g�ǉ�
		actor->AddComponent<Stage>();
		actor->AddComponent<StageObjectCollision>(ActorType::Conifer);
		// �v���C���[�̃J�����O�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(object_data->object_category, actor));
		// �V�F�[�_�[�ݒ�
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}


}

//--------------------------------
// �X�e�[�W�I�u�W�F�N�g�o�^����
//--------------------------------
void StageManager::WorldObjectRegister()
{
	// �f�[�^�x�[�X�C���X�^���X�擾
	GameDataBase& data_base = GameDataBase::Instance();
	// �X�e�[�W�I�u�W�F�N�g�f�[�^���擾
	std::vector<std::shared_ptr<StageObjectData>> stage_object_list = data_base.GetButtleMapObjectDataList();
	// �o�g���}�b�v�̃I�u�W�F�N�g�z�u�ʒu�f�[�^���擾
	std::vector<std::shared_ptr<WorldMapObjectPositionData>> object_position_list = data_base.GetWorldMapObjectPositionDataList();
	// �f�[�^�̃T�C�Y�擾
	int data_max_count = data_base.GetWorldMapDataCount();

	// �I�u�W�F�N�g��������
	for (int i = 0; i < data_max_count; ++i)
	{
		// �I�u�W�F�N�g�z�u�f�[�^���擾
		std::shared_ptr<WorldMapObjectPositionData> object_position_data = object_position_list.at(i);
		// �I�u�W�F�N�g�̔z�u�f�[�^����I�u�W�F�N�g�f�[�^���擾����
		std::shared_ptr<StageObjectData> object_data = data_base.GetButtleMapObjectData(object_position_data->object_category);
		// �A�N�^�[���쐬
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		// ���f���ǂݍ���
		actor->SetUpModel(object_data->model_path, nullptr);
		// �p�x�ݒ�
		actor->SetAngle({ object_data->angle_x, object_data->angle_y, object_data->angle_z });
		// �X�P�[���ݒ�
		actor->SetScale({ object_data->scale_x, object_data->scale_y, object_data->scale_z });
		// ���O�ݒ�
		actor->SetName(object_position_data->name);
		// �ʒu�ݒ�
		actor->SetPosition({ object_position_data->position_x, object_position_data->position_y, object_position_data->position_z });
		// �R���|�[�l���g�ǉ�
		actor->AddComponent<Stage>();
		actor->AddComponent<StageObjectCollision>(ActorType::Conifer);
		// �v���C���[�̃J�����O�R���W������ǉ�
		CollisionManager::Instance().RegisterCulling(std::make_shared<CullingCollision>(object_data->object_category, actor));
		// �V�F�[�_�[�ݒ�
		actor->SetShaderType(ShaderManager::ShaderType::Phong);
	}


}

//---------------------------------
// �X�e�[�W�I�u�W�F�N�g�o�^��������
//---------------------------------
void StageManager::Unregister(std::shared_ptr<Actor> object)
{
	// �C�e���[�^�[�z�񂩂�I�u�W�F�N�g������
	std::vector<std::shared_ptr<Actor>>::iterator iterator_object = std::find(stage_objects.begin(), stage_objects.end(), object);
	// �C�e���[�^�[���擾�o������폜
	if (iterator_object != stage_objects.end())
	{
		stage_objects.erase(iterator_object);
	}
}

//--------------------------------
// �j������
//--------------------------------
void StageManager::Destroy()
{
	// �z��̑S�Ă�j��
	std::vector<std::shared_ptr<Actor>>::iterator iterator_object = stage_objects.begin();
	for (; iterator_object != stage_objects.end(); iterator_object = stage_objects.begin())
	{
		stage_objects.erase(iterator_object);
	}
}

//--------------------------------
// �X�V����
//--------------------------------
void StageManager::Update(float elapsed_time)
{
}

