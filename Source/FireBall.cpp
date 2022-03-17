#include <vector>
#include "Mathf.h"

#include "MetaAI.h"

#include "FireBall.h"
#include "Effect.h"

// �}�l�[�W���[�C���N���[�h
#include "ActorManager.h"
#include "CollisionManager.h"
#include "FireBallManager.h"

#include "Charactor.h"
#include "Enemy.h"

#include "Model.h"

#include "GameDatabase.h"
//******************************
// 
// �΋��N���X
// 
//******************************
//-------------------------------
// �J�n����
//-------------------------------
void FireBall::Start()
{
    // �L�����N�^�[�̎擾
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // �����蔻��ݒ�
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(EnemyCategory::FireBall);
    CollisionParameter parameter;
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        parameter.actor_id = charactor->GetID();
        parameter.radius = data->radius;
        parameter.weight = data->weight;
        parameter.height = data->height;
        parameter.local_position.x = data->local_x;
        parameter.local_position.y = data->local_y;
        parameter.local_position.z = data->local_z;
        parameter.collision_flg = data->collision_flag;
        parameter.actor_type = data->actor_type;
        parameter.update_type = data->collision_update_type;

        collision_sphere = std::make_shared<CollisionSphere>(parameter);

        // ���̃R���W�������}�l�[�W���[�ɐݒ�
        CollisionManager::Instance().RegisterSphere(collision_sphere);
    }

    // ���f���擾
    Model* model = actor->GetModel();

    // �m�[�h�擾
    Mathf::GetNodePosition(parameter.node_name.c_str(), fireball_position, model);

    // �G�t�F�N�g�}�l�[�W���[������
    effekseer_manager = std::make_unique<EffectManager>();
    effekseer_manager->Initialize();

    // �G�t�F�N�g�ǂݍ���
    fireball_effect = std::make_unique<Effect>("Data/Effect/test.efk", effekseer_manager->GetEffekseerManager());

    fireball_position.y = 9;
    collision_sphere->SetPosition(fireball_position);
}

//-------------------------------
// �X�V����
//-------------------------------
void FireBall::Update(float elapsed_time)
{
    // ���f���擾
    Model* model = actor->GetModel();

    // ��VS�~��
    int cylinder_count = CollisionManager::Instance().GetCollisionCylinderCount();
    for (int j = 0; j < cylinder_count; j++)
    {
        std::shared_ptr<CollisionCylinder> cylinder = CollisionManager::Instance().GetCollisionCylinder(j);
        // �A�N�^�[���v���C���[�Ȃ��΂�
        if (collision_sphere->GetActorType() == cylinder->GetActorType()) continue;

        // �����蔻��t���O�������Ă��Ȃ��������΂�
        if (!cylinder->GetCollisionFlag()) continue;
        ObjectCollisionResult result;
        if (CollisionManager::Instance().IntersectSphereVsCylinder(collision_sphere.get(), cylinder.get(), result))
        {
            // �~���̃A�N�^�[�擾
            std::shared_ptr<Actor> cylinder_actor = ActorManager::Instance().
            GetActor(cylinder->GetActorName());

            Message message;
            // �U���������������Ƃ̃��b�Z�[�W
            cylinder_actor->GetComponent<Charactor>()->ApplyDamage(attack, 0.0f);

            // �U���𓖂Ă����Ƃ̃��b�Z�[�W
            // �G�R���|�[�l���g�擾
            std::shared_ptr<Enemy> enemy = actor->GetComponent<Enemy>();
            message.message = MessageType::Message_Hit_Attack;
            message.hit_position = { 0.0f, 0.0f, 0.0f };
            Reaction(collision_sphere->GetActorID() + enemy->GetIdentity(), message);
        }
    }

    // �΋��G�t�F�N�g�̍��W����
    float speed = this->speed * elapsed_time;
    fireball_position.x += direction.x * speed;
    fireball_position.z += direction.z * speed;
    // �G�t�F�N�g�Đ�
    fireball_effect->Play(effekseer_manager->GetEffekseerManager(), fireball_position, fireball_scale);

    // �G�t�F�N�g�}�l�[�W���[�X�V����
    effekseer_manager->Update(elapsed_time);
    effekseer_manager->GetEffekseerManager()->StopAllEffects();
    // �����蔻��̍��W�ݒ�
    collision_sphere->SetPosition(fireball_position);

    life_timer -= elapsed_time;

    if (life_timer <= 0.0f)
    {
        FireBallManager::Instance().Unregister(shared_from_this());
        CollisionManager::Instance().UnregisterSphere(collision_sphere);
    }
}

//-------------------------------
// �`�揈��
//-------------------------------
void FireBall::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // �`�揈��
    effekseer_manager->Render(view, projection);
}

//-------------------------------
// �Փˎ��̃��A�N�V��������
//-------------------------------
void FireBall::Reaction(int receiver, const Message& message)
{
    // ����ƏՓ˂����R���W�����̎�����ɏՓ˂������Ƃ𑗐M����
    MetaAI::Instance().SendMessaging(
        static_cast<int>(MetaAI::Identity::Collision), // ���M��
        receiver,                                // ��M��
        message);                                      // ���b�Z�[�W
}
