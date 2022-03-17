#pragma once
#include <string>
#include <memory>

enum class EnemyCategory;
struct CollisionParameter;
class CollisionBox;
class Actor;
class DebugRenderer;

//************************************
// 
// �v���C���[�̃J�����O�R���W����
// 
//************************************
class CullingCollision
{
public:
	// �R���X�g���N�^
	CullingCollision(EnemyCategory enemy_category, std::shared_ptr<Actor> actor);

	// �f�X�g���N�^
	~CullingCollision();

	// ���O�擾
	std::string GetName() const  { return this->name; }

	// GUI�`��
	void OnGUI();

	// �J�n����
	void Start();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(DebugRenderer* renderer);

	// �Փˎ��̃��A�N�V��������
	void Reaction(bool flag);

	// �R���W�����擾
	std::shared_ptr<CollisionBox> GetCullingCollisiton() { return collision_culling; }

private:
	std::shared_ptr<Actor> actor;
	std::shared_ptr<CollisionBox> collision_culling;
	std::string name = "";
	EnemyCategory	enemy_category; // �f�[�^�x�[�X�̃R���W�����f�[�^�����Ɏg��
};
