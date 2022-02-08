#include "Collision.h"
#include "CullingCollision.h"
#include "CollisionManager.h"
#include "Universal.h"
#include "SceneManager.h"

//************************************
// 
// �v���C���[�̃J�����O�R���W����
// 
//************************************
//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
CullingCollision::CullingCollision(EnemyCategory enemy_category, std::shared_ptr<Actor> actor)
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
    Universal::NodePositionUpdate(collision_culling.get(), collision_culling->GetNodeName(), model);

    // �J�����O���s�����̔���
    std::vector<Plane> frustum = SceneManager::Instance().GetCurrentScene()->GetCameraController()->GetFrustum();
    Reaction(CollisionManager::Instance().IntersectFrustumVsAABB(collision_culling, frustum));
}

//-----------------------------------
// �`�揈��
//-----------------------------------
void CullingCollision::Render(DebugRenderer* renderer)
{
    if (!collision_culling->GetAttackFlag()) return;
    collision_culling->Render(renderer);
}

//-----------------------------------
// �Փˎ��̃��A�N�V��������
//-----------------------------------
void CullingCollision::Reaction(bool flag)
{
    actor->SetCullingFlag(flag);
}
