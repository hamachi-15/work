// �}�l�[�W���[�C���N���[�h
#include "StageManager.h"
#include "ActorManager.h"

// �A�N�^�[�E�R���|�[�l���g�C���N���[�h
#include "Actor.h"
#include "Stage.h"

// �v�Z�C���N���[�h
#include "Mathf.h"

// �f�[�^�x�[�X�C���N���[�h
#include "GameDatabase.h"

//*************************************************
// 
// �X�e�[�W�I�u�W�F�N�g�̃}�l�[�W���[�N���X
// 
//*************************************************
//--------------------------------
// �X�e�[�W�I�u�W�F�N�g�o�^����
//--------------------------------
void StageManager::Register()
{
	// �f�[�^�x�[�X�C���X�^���X�擾
	GameDataBase& data_base = GameDataBase::Instance();
	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	actor->SetUpModel("Data/Model/Filde/StageObjects.mdl", nullptr);
	actor->SetName("FildeObjects");
	actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
	actor->SetAngle(DirectX::XMFLOAT3(0, Mathf::ConvartToRadian(-90), 0));
	actor->SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
	actor->AddComponent<Stage>();
	actor->SetShaderType(ShaderManager::ShaderType::Lambert);

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

