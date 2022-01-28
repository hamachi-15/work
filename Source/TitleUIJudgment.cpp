#include "TitleUIJudgment.h"
#include "SceneManager.h"
#include "TitleButtonUI.h"
#include "UIManager.h"

//*********************************************
// 
// �X�^�[�g�{�^���̑I�����`����s�����̔���N���X
// 
//*********************************************
//--------------------------------
// ����
//--------------------------------
bool StartButtonSelectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Start_Button) == TitleUI::SelectState::Select_Game_Start);
}

//*********************************************
// 
// �X�^�[�g�{�^���̔�I�����`����s�����̔���N���X
// 
//*********************************************
//--------------------------------
// ����
//--------------------------------
bool StartButtonUnselectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Start_Button) == TitleUI::SelectState::Unselect_Game_Start);
}

//*****************************************
// 
// ����{�^���̑I�����`����s�����̔���N���X
// 
//*****************************************
//--------------------------------
// ����
//--------------------------------
bool OperateButtonSelectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Operate_Button) == TitleUI::SelectState::Select_Game_Start);
}

//********************************************
// 
// ����{�^���̔�I�����`����s�����̔���N���X
// 
//********************************************
//--------------------------------
// ����
//--------------------------------
bool OperateButtonUnselectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Operate_Button) == TitleUI::SelectState::Unselect_Game_Start);
}
