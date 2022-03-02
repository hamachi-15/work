#pragma once
#include <Effekseer.h>
#include "Collision.h"
#include "Effect.h"
#include "EffectManager.h"

class Actor;

//******************************
// 
// �΋��N���X
// 
//******************************
class FireBall : public std::enable_shared_from_this<FireBall>
{
public:
	// �R���X�g���N�^
	FireBall(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 direction) : actor(actor), direction(direction){}
	
	// �f�X�g���N�^
	~FireBall() {}

	// �J�n����
	void Start();

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// �Փˎ��̃��A�N�V��������
	void Reaction(int receiver, const Message& message);
private:
	std::shared_ptr<Actor>				actor;
	std::unique_ptr<Effect>				fireball_effect;	// �΋��̃G�t�F�N�g
	std::unique_ptr<EffectManager>		effekseer_manager;
	std::shared_ptr<CollisionSphere>	collision_sphere;	// �����蔻��
	Effekseer::Handle					handle;				// �G�t�F�N�g�̃n���h��
	
	DirectX::XMFLOAT3					fireball_position;	// �����ʒu
	float								fireball_scale = 5.0f;		// �G�t�F�N�g�̃X�P�[��
	DirectX::XMFLOAT3					direction;			// �i�s�x�N�g��
	float								speed = 100.0f;		// �΋��̈ړ����x

	float								attack = 5.0f;		// �U����
	float								life_timer = 4.0f;	// ��������
};