#pragma once
#include "UI.h"
#include "UIData.h"

//********************************
// 
// �^�C�g���̃{�^��UI
// 
//********************************
class TitleButtonUI : public UI
{
public:
	enum class SelectState;
	enum class ButtonType;
public:
	// �R���X�g���N�^
	TitleButtonUI() {}

	// �f�X�g���N�^
	~TitleButtonUI() override;

	// ���O�擾
	std::string GetName() const override { return "TitleButtonUI"; }

	// �J�n����
	void Start() override;

	// �{�^���̑I����Ԑݒ�
	void SetSelectState(ButtonType index, SelectState select_state) { select_button[static_cast<int>(index)] = select_state; }

	// �{�^���̑I����Ԏ擾
	SelectState GetButtonState(ButtonType index)const { return select_button[static_cast<int>(index)]; }
public:
	enum class SelectState
	{
		Unselect_Game_Start,
		Select_Game_Start,
	};
	enum class ButtonType
	{
		Start_Button,
		Operate_Button,
		Button_Max
	};
private:
	SelectState select_button[static_cast<int>(ButtonType::Button_Max)];

	UIData* start_button_data;
	UIData* select_start_button_data;
	UIData* start_data;

	UIData* operate_button_data;
	UIData* select_operate_button_data;
	UIData* operate_data;
};