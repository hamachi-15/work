#include "PlayerUIRedHealth.h"
#include "Charactor.h"

//--------------------------------
// 更新処理
//--------------------------------
void PlayerUIRedHealth::Update(float elapsed_time)
{
	DirectX::XMFLOAT2 size = GetSize();
	DirectX::XMFLOAT2 parent_size = GetParent()->GetSize();
	// HPゲージが減った時、徐々に減少していく
	if (parent_size.x <= size.x)
	{
		size.x -= speed * elapsed_time;
		SetSize(size);
	}
		
}
