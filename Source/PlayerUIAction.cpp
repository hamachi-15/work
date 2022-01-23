#include "PlayerUIAction.h"
#include "UIData.h"
#include "Charactor.h"

//******************************************
// 
// �v���C���[�̗΃Q�[�W�̃A�N�V����
// 
//******************************************
//-----------------------------------------
// �A�N�V����
//-----------------------------------------
void PlayerGreenHealthAction::Action(float elapsed_time)
{
	std::shared_ptr<Charactor> charactor = ui_data->GetCharactor();
	// �v���C���[��HP����Q�[�W�̒������Z�o
	float health = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	DirectX::XMFLOAT2 size = ui_data->GetSize();
	size.x = ui_data->GetTexture()->GetWidth() * health;
	ui_data->SetSize(size);
}

//******************************************
// 
// �v���C���[�̐ԃQ�[�W�̃A�N�V����
// 
//******************************************
//-----------------------------------------
// �A�N�V����
//-----------------------------------------
void PlayerRedHealthAction::Action(float elapsed_time)
{
	std::shared_ptr<Charactor> charactor = ui_data->GetCharactor();
	float health_percent = static_cast<float>(charactor->GetHealth()) / static_cast<float>(charactor->GetMaxHealth());
	float health_size = ui_data->GetTexture()->GetWidth() * health_percent;
	DirectX::XMFLOAT2 size = ui_data->GetSize();
	// HP�Q�[�W�����������A���X�Ɍ������Ă���
	if (health_size <= size.x)
	{
		size.x -= speed * elapsed_time;
		ui_data->SetSize(size);
	}
}
