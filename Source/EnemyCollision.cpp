#include "EnemyCollision.h"
// ���b�Z�[�W
#include "Telegram.h"

// AI
#include "MetaAI.h"

// ���f��
#include "Model.h"

// �`��
#include "DebugRenderer.h"
#include "Graphics.h"

// �}�l�[�W���[
#include "ActorManager.h"
#include "CollisionManager.h"
#include "SceneManager.h"

// �R���|�[�l���g
#include "Charactor.h"
#include "Enemy.h"

// �G�t�F�N�g
#include "Effect.h"
#include "EffectManager.h"

// �ėp�֐�
#include "Universal.h"

// �Q�[���f�[�^
#include "GameDatabase.h"
#include "CollisionParameterData.h"
//************************************
// 
// �v���C���[�̃R���W����
// 
//************************************
//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
EnemyCollision::~EnemyCollision()
{
    // ���R���W�����̔j������
    std::vector<std::shared_ptr<CollisionSphere>>::iterator iterate_sphere = collision_spheres.begin();
    for (; iterate_sphere != collision_spheres.end(); iterate_sphere = collision_spheres.begin())
    {
        collision_spheres.erase(iterate_sphere);
    }
    collision_spheres.clear();

    // �}�l�[�W���[�̋��R���W�����폜
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {  
        // ���R���W�����폜
    	CollisionManager::Instance().UnregisterSphere(sphere);
    }

    // �}�l�[�W���[�̉~���R���W�����폜
    // �~���R���W�����폜
    CollisionManager::Instance().UnregisterCylinder(collision_cylinder);
}

//-----------------------------------
// GUI�`��
//-----------------------------------
void EnemyCollision::OnGUI()
{

}

//-----------------------------------
// �J�n����
//-----------------------------------
void EnemyCollision::Start()
{
    // �A�N�^�[�̎擾
    std::shared_ptr<Actor> actor = GetActor();

    // �L�����N�^�[�̎擾
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // �G�t�F�N�g�}�l�[�W���[�擾
    std::shared_ptr<EffectManager> effect_manager = Graphics::Instance().GetEffectManager();

    // �G�t�F�N�g�ǂݍ���
    hit_effect = std::make_shared<Effect>("Data/Effect/Hit.efk", effect_manager->GetEffekseerManager());

    // ���f���擾
    Model* model = actor->GetModel();
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(enemy_category);
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        parameter.actor_id = charactor->GetID() + identity;
        parameter.radius = data->radius;
        parameter.weight = data->weight;
        parameter.height = data->height;
        parameter.local_position.x = data->local_x;
        parameter.local_position.y = data->local_y;
        parameter.local_position.z = data->local_z;
        parameter.collision_flg = data->collision_flag;
        parameter.actor_type = data->actor_type;
        parameter.update_type = data->collision_update_type;
        // �f�[�^�̃R���W�����^�C�v���~���Ȃ�
        if (data->collision_type == CollisionMeshType::Cylinder)
        {
            collision_cylinder = std::make_shared<CollisionCylinder>(parameter);
            // �~���̃R���W�������}�l�[�W���[�ɐݒ�
            CollisionManager::Instance().RegisterCylinder(collision_cylinder);
        }
        // �f�[�^�̃R���W�����^�C�v�����Ȃ�
        else if (data->collision_type == CollisionMeshType::Sphere)
        {
            collision_spheres.emplace_back(std::make_shared<CollisionSphere>(parameter));
            // ���̃R���W�������}�l�[�W���[�ɐݒ�
            CollisionManager::Instance().RegisterSphere(collision_spheres.back());
        }
    }
}

//-----------------------------------
// �X�V����
//-----------------------------------
void EnemyCollision::Update(float elapsed_time)
{
    // �A�N�^�[�擾
    std::shared_ptr<Actor> actor = GetActor();

    // ���f���擾
    Model* model = GetActor()->GetModel();

    // �w�肳�ꂽ�m�[�h�̃��[�J�����W�ɍX�V
    for (std::shared_ptr<CollisionSphere> sphere : collision_spheres)
    {
        // �R���W�����̍��W�X�V�̃^�C�v�ɍ��킹�čX�V
        UpdateCollision(sphere, actor, model);
    }

    // �R���W�����̍��W�X�V�̃^�C�v�ɍ��킹�čX�V
    UpdateCollision(collision_cylinder, actor, model);
    std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();

    // ���R���W�����擾
    int sphere_count = static_cast<int>(collision_spheres.size());

    // �}�l�[�W���[�̉~���̐��擾
    int cylinder_count = static_cast<int>(CollisionManager::Instance().GetCollisionCylinderCount());

    // �G�̋��R���W�����ƃ}�l�[�W���[�ɐݒ肳��Ă���~���R���W�����̓����蔻��
    for (int i = 0; i < sphere_count; i++)
    {
        // ���R���W�����擾
        CollisionSphere* sphere = collision_spheres.at(i).get();

        // �U���t���O�������Ă��Ȃ��������΂�
        if (!sphere->GetAttackFlag()) continue;

        // �U�����������Ă����ꍇ��΂�
        if (charactor->GetHitAttackFlag()) break;

        for (int j = 0; j < cylinder_count; j++)
        {
            std::shared_ptr<CollisionCylinder> cylinder = CollisionManager::Instance().GetCollisionCylinder(j);
            // �����A�N�^�[���m�̓����蔻��Ȃ��΂�
            if (sphere->GetActorType() == cylinder->GetActorType()) continue;

            // �����蔻��t���O�������Ă��Ȃ��������΂�
            if (!cylinder->GetCollisionFlag()) continue;
            // ���Ɖ~�����������Ă�����
            ObjectCollisionResult result;
            if (CollisionManager::Instance().IntersectSphereVsCylinder(sphere, cylinder.get(), result))
            {
                // �~���R���W�����̃A�N�^�[�擾
                std::shared_ptr<Actor> cylinder_actor = ActorManager::Instance().GetActor(cylinder->GetActorName());

                // �~���R���W�����������Ă���L�����N�^�[�Ƀ_���[�W��^����
                cylinder_actor->GetComponent<Charactor>()->ApplyDamage(charactor->GetAttack(), 0.8f);

                // �~���R���W���������A�N�^�[�ʒu�擾
                DirectX::XMFLOAT3 cylinder_actor_position = cylinder_actor->GetPosition();

                // �G�t�F�N�g�̍Đ��ʒu��ݒ�
                DirectX::XMFLOAT3 diff;
                DirectX::XMStoreFloat3(&diff, result.vector);
                DirectX::XMFLOAT3 sphere_position = sphere->GetPosition();
                DirectX::XMFLOAT3 play_position = {
                    sphere_position.x - diff.x,
                    sphere_position.y - diff.y,
                    sphere_position.z - diff.z };
                // �q�b�g�G�t�F�N�g�Đ�
                hit_effect->Play(Graphics::Instance().GetEffectManager()->GetEffekseerManager(),
                    play_position, hit_effect_scale);

                // ���A�N�V���������b�Z�[�W�ɑ���
                Message message;
                message.message = MessageType::Message_Hit_Attack;
                message.hit_position = { 0.0f, 0.0f, 0.0f };
                Reaction(sphere->GetActorID(), message);
            }
        }
    }


    // �����蔻��t���O�������Ă��Ȃ��������΂�
    if (!collision_cylinder->GetCollisionFlag()) return;

    // �~��Vs�~��
    ObjectCollisionResult result;
    for (int j = 0; j < cylinder_count; j++)
    {
        std::shared_ptr<CollisionCylinder> cylinderB = CollisionManager::Instance().GetCollisionCylinder(j);
        // �����蔻��t���O�������Ă��Ȃ��������΂�
        if (!cylinderB->GetCollisionFlag()) continue;

        // �����A�N�^�[���m�̓����蔻��Ȃ��΂�
        if (collision_cylinder->GetActorType() == cylinderB->GetActorType()) continue;

        std::shared_ptr<Actor> cylinderB_actor = ActorManager::Instance().GetActor(cylinderB->GetActorName());
        // �~�����m���������Ă�����
        if (CollisionManager::Instance().IntersectCylinderVsCylinder(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result))
        {
            CollisionManager::Instance().PushOutCollision(collision_cylinder.get(), GetActor(), cylinderB.get(), cylinderB_actor, result);
        }
    }

}

//--------------------------------
// �R���W�����X�V����
//--------------------------------
void EnemyCollision::UpdateCollision(std::shared_ptr<CollisionObject> collision, std::shared_ptr<Actor> actor, Model* model)
{
    // �R���W�����̍��W�X�V�̃^�C�v�ɍ��킹�čX�V
    switch (collision->GetUpdateType())
    {
    case CollisionUpdateType::Update_Actor:
        // �A�N�^�[���W�ɍX�V
        Universal::ActorPositionUpdate(collision.get(), actor);
        break;
    case CollisionUpdateType::Update_Node:
        // ����m�[�h���W�ɍX�V
        Universal::NodePositionUpdate(collision.get(), collision->GetNodeName(), model);
        break;
    case CollisionUpdateType::Update_Local:
        // ����m�[�h���W�̃��[�J���ʒu���Z�o���X�V
        Universal::LocalPositionUpdate(collision.get(), model->FindNode(collision->GetNodeName()));
        break;
    case CollisionUpdateType::Update_Castam:
        // ���̑��̍X�V���@
        Universal::CastamPositionUpdate(collision.get(), actor, collision->GetNodeName(), model);
        break;
    }
}

//-----------------------------------
// �Փˎ��̃��A�N�V��������
//-----------------------------------
void EnemyCollision::Reaction(int receiver, const Message& message)
{
    // ����ƏՓ˂����R���W�����̎�����ɏՓ˂������Ƃ𑗐M����
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Collision),  // ���M��
        receiver,                                       // ��M��
        message);                                       // ���b�Z�[�W
}
