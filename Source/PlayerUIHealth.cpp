#include "PlayerUIHealth.h"
#include "Texture.h"
#include "Model.h"

// デストラクタ
PlayerHealth::~PlayerHealth()
{
}

// 更新処理
void PlayerHealth::Update(float elapsed_time)
{
	// プレイヤーのHPからゲージの長さを算出
	float health = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	DirectX::XMFLOAT2 size = GetSize();
	size.x = GetTexture()->GetWidth() * health;
	SetSize(size);

	// 親が存在すれば親の座標に合わせて移動
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
