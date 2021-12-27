#include "PlayerUIHealth.h"
#include "Texture.h"
#include "Model.h"

// �f�X�g���N�^
PlayerHealth::~PlayerHealth()
{
}

// �X�V����
void PlayerHealth::Update(float elapsed_time)
{
	// �v���C���[��HP����Q�[�W�̒������Z�o
	float health = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	DirectX::XMFLOAT2 size = GetSize();
	size.x = GetTexture()->GetWidth() * health;
	SetSize(size);

	// �e�����݂���ΐe�̍��W�ɍ��킹�Ĉړ�
	//std::shared_ptr<UI> parent = GetParent();
	//if (parent != nullptr)
	//{
	//	DirectX::XMFLOAT2 parent_postion = parent->GetPosition();
	//	DirectX::XMFLOAT2 child_postion = GetPosition();
	//	DirectX::XMFLOAT2 position;
	//	position.x = parent_postion.x + child_postion.x;
	//	position.y = parent_postion.y + child_postion.y;
	//	SetPosition(position);
	//}
}
