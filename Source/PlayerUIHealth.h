#pragma once
#include <memory>
#include "UI.h"
#include "UIData.h"
#include "Component.h"


//*************************************
// 
// �v���C���[HP�N���X
// 
//*************************************
class PlayerHealthUI : public UI, public Component
{
public:
	// �R���X�g���N�^
	PlayerHealthUI(){}

	// �f�X�g���N�^
	~PlayerHealthUI() override {}
	
	// ���O�擾
	std::string GetName() const override { return "PlayerHealthUI"; }
	
	// �J�n����
	void Start() override;

private:
	std::unique_ptr<UIData> health_red_gauge_data;
	std::unique_ptr<UIData> health_green_gauge_data;
	std::unique_ptr<UIData> health_gauge_base_data;
	std::unique_ptr<UIData> health_gauge_freame_data;
};