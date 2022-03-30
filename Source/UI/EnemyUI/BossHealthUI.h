#pragma once
#include <memory>
#include "UI.h"
#include "UIData.h"
#include "Component.h"


//**********************************
// 
// �{�X�N���X�̓GHPUI�N���X
// 
//**********************************
class BossHealthUI : public UI, public Component
{
public:
	// �R���X�g���N�^
	BossHealthUI() {}
	
	// �f�X�g���N�^
	~BossHealthUI() override {}

	// ���O�擾
	std::string GetName() const override { return "BossHealthUI"; }
	
	// �J�n����
	void Start() override;

	std::unique_ptr<UIData> health_gage_freame_data;
	std::unique_ptr<UIData> health_base_data;
	std::unique_ptr<UIData> health_gold_gage_data;
	std::unique_ptr<UIData> health_red_gage_data;
};