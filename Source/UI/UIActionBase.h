#pragma once
// �O���錾
class UIData;

//****************************
// 
// UI�x�[�X�N���X
// 
//****************************
class UIActionBase
{
public:
	// �R���X�g���N�^
	UIActionBase(UIData* ui_data) : ui_data(ui_data) {}

	// �A�N�V����(�������z�֐�)
	virtual void Action(float elapsed_time) = 0;

protected:
	UIData* ui_data;
};