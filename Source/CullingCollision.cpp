#include <string>
#include "Collision.h"
#include "CullingCollision.h"
#include "CollisionManager.h"
#include "Universal.h"
#include "SceneManager.h"

// �Q�[���f�[�^
#include "GameDatabase.h"
#include "ActorType.h"
#include "CullingCollisionParameterData.h"
//************************************
// 
// �J�����O�R���W����
// 
//************************************
//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
CullingCollision::CullingCollision(ActorType enemy_category, std::shared_ptr<Actor> actor)
{
    // �A�N�^�[�ݒ�
    this->actor = actor;

    // ���f���擾
    Model* model = this->actor->GetModel();

    std::vector<std::shared_ptr<CullingCollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCullingCollisionParameterDataList(enemy_category);
    for (std::shared_ptr<CullingCollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        parameter.name = this->actor->GetName();
        parameter.actor_name = this->actor->GetName();
        parameter.node_name = data->node_name;
        parameter.xmfloat_radius = { data->radius_x, data->radius_y,data->radius_z };
        parameter.local_position = { data->local_x, data->local_y, data->local_z };
        DirectX::XMFLOAT3 node_position = {};
        parameter.update_type = data->update_type;
        //Mathf::GetNodePosition(parameter.node_name.c_str(), node_position, model);
        //parameter.position = { node_position.x, node_position.y + 0.5f, node_position.z };
        collision_culling = std::make_shared<CollisionBox>(parameter);
    }
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
CullingCollision::~CullingCollision()
{
}

//-----------------------------------
// GUI�`��
//-----------------------------------
void CullingCollision::OnGUI()
{
}

//-----------------------------------
// �J�n����
//-----------------------------------
void CullingCollision::Start()
{
}

//-----------------------------------
// �X�V����
//-----------------------------------
void CullingCollision::Update()
{
    // ���f���擾
    Model* model = actor->GetModel();

    // �w�肳�ꂽ�m�[�h�̃��[�J�����W�ɍX�V
    // �X�V���@�����̑��ɐݒ肳��Ă��Ȃ����
    if (collision_culling->GetUpdateType() == CollisionUpdateType::Update_Node)
    {// ����̃m�[�h�̈ʒu�ɍX�V
        Universal::NodePositionUpdate(collision_culling.get(), collision_culling->GetNodeName(), model);
    }
    else if (collision_culling->GetUpdateType() == CollisionUpdateType::Update_Local)
    {
        Model::Node* node = model->FindNode(collision_culling->GetNodeName());
        Universal::LocalPositionUpdate(collision_culling.get(), node);
    }

    // �J�����O���s�����̔���
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    Reaction(CollisionManager::Instance().IntersectFrustumVsAABB(collision_culling, frustum));
}

//-----------------------------------
// �`�揈��
//-----------------------------------
void CullingCollision::Render(DebugRenderer* renderer)
{
    // �f�o�b�O�v���~�e�B�u�`��
    collision_culling->Render(renderer);
}

//-----------------------------------
// �Փˎ��̃��A�N�V��������
//-----------------------------------
void CullingCollision::Reaction(bool flag)
{
    // �J�����O�t���O�̐ݒ�
    actor->SetCullingFlag(flag);
}
