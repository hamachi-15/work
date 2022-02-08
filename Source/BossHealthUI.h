#pragma once
#include "UI.h"
#include "Component.h"
#include "UIData.h"
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
	~BossHealthUI() override { 
		delete health_gage_freame_data;
		delete health_base_data;
		delete health_gold_gage_data;
		delete health_red_gage_data;
	}

	// ���O�擾
	std::string GetName() const override { return "BossHealthUI"; }
	
	// �J�n����
	void Start() override;

	UIData* health_gage_freame_data;
	UIData* health_base_data;
	UIData* health_gold_gage_data;
	UIData* health_red_gage_data;
};