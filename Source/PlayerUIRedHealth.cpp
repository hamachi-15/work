#include "PlayerUIRedHealth.h"
#include "Charactor.h"

//--------------------------------
// �X�V����
//--------------------------------
void PlayerUIRedHealth::Update(float elapsed_time)
{
	DirectX::XMFLOAT2 size = GetSize();
	DirectX::XMFLOAT2 parent_size = GetParent()->GetSize();
	// HP�Q�[�W�����������A���X�Ɍ������Ă���
	if (parent_size.x <= size.x)
	{
		size.x -= speed * elapsed_time;
		SetSize(size);
	}
		
}
