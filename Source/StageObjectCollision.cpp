#include "StageObjectCollision.h"
// �f�[�^�x�[�X
#include "GameDatabase.h"
#include "ActorType.h"
#include "CollisionParameterData.h"

// �}�l�[�W���[
#include "CollisionManager.h"

// �R���|�[�l���g
#include "Actor.h"
#include "Charactor.h"

// �ėp�֐�
#include "Universal.h"

//*************************************
// 
// �I�u�W�F�N�g�̃R���W����
// 
//*************************************
//-----------------------
// GUI�`��
//-----------------------
void StageObjectCollision::OnGUI()
{
}

//-----------------------
// �J�n����
//-----------------------
void StageObjectCollision::Start()
{
    // �A�N�^�[�̎擾
    std::shared_ptr<Actor> actor = GetActor();

    // �L�����N�^�[�̎擾
    std::shared_ptr<Charactor> charactor = actor->GetComponent<Charactor>();

    // �R���W�����̃f�[�^����R���W������ݒ�
    std::vector<std::shared_ptr<CollisionParameterData>> collision_parameter = GameDataBase::Instance().GetAttackCollitionParamterDataList(category);
    for (std::shared_ptr<CollisionParameterData> data : collision_parameter)
    {
        CollisionParameter parameter;
        std::string collision_name = actor->GetName();
        parameter.name = collision_name + data->collision_name;
        parameter.actor_name = actor->GetName();
        parameter.node_name = data->node_name;
        //parameter.actor_id = charactor->GetID();
        parameter.radius = data->radius;
        parameter.weight = data->weight;
        parameter.height = data->height;
        parameter.local_position.x = data->local_x;
        parameter.local_position.y = data->local_y;
        parameter.local_position.z = data->local_z;
        parameter.collision_flg = data->collision_flag;
        parameter.actor_type = data->actor_type;
        parameter.update_type = data->collision_update_type;
        if (data->collision_type == CollisionMeshType::Cylinder)
        {
            collision_cylinder = std::make_shared<CollisionCylinder>(parameter);
            // �~���̃R���W�������}�l�[�W���[�ɐݒ�
            CollisionManager::Instance().RegisterCylinder(collision_cylinder);
        }
    }
}

//-----------------------
// �X�V����
//-----------------------
void StageObjectCollision::Update(float elapsed_time)
{
    // �A�N�^�[�擾
    std::shared_ptr<Actor> actor = GetActor();

    // �A�N�^�[���W�ɃR���W�������X�V
    Universal::ActorPositionUpdate(collision_cylinder.get(), actor);
}
