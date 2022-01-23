#include "PlayerUIAction.h"
#include "UIData.h"
#include "Charactor.h"

//******************************************
// 
// プレイヤーの緑ゲージのアクション
// 
//******************************************
//-----------------------------------------
// アクション
//-----------------------------------------
void PlayerGreenHealthAction::Action(float elapsed_time)
{
	std::shared_ptr<Charactor> charactor = ui_data->GetCharactor();
	// プレイヤーのHPからゲージの長さを算出
	float health = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	DirectX::XMFLOAT2 size = ui_data->GetSize();
	size.x = ui_data->GetTexture()->GetWidth() * health;
	ui_data->SetSize(size);
}

//******************************************
// 
// プレイヤーの赤ゲージのアクション
// 
//******************************************
//-----------------------------------------
// アクション
//-----------------------------------------
void PlayerRedHealthAction::Action(float elapsed_time)
{
	std::shared_ptr<Charactor> charactor = ui_data->GetCharactor();
	float health_percent = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	float health_size = ui_data->GetTexture()->GetWidth() * health_percent;
	DirectX::XMFLOAT2 size = ui_data->GetSize();
	// HPゲージが減った時、徐々に減少していく
	if (health_size <= size.x)
	{
		size.x -= speed * elapsed_time;
		ui_data->SetSize(size);
	}
}
