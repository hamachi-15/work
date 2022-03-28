#include "TitleUIJudgment.h"
#include "SceneManager.h"
#include "TitleButtonUI.h"
#include "UIManager.h"

//*********************************************
// 
// スタートボタンの選択時描画を行うかの判定クラス
// 
//*********************************************
//--------------------------------
// 判定
//--------------------------------
bool StartButtonSelectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Start_Button) == TitleUI::SelectState::Select_Game_Start);
}

//*********************************************
// 
// スタートボタンの非選択時描画を行うかの判定クラス
// 
//*********************************************
//--------------------------------
// 判定
//--------------------------------
bool StartButtonUnselectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Start_Button) == TitleUI::SelectState::Unselect_Game_Start);
}

//*****************************************
// 
// 操作ボタンの選択時描画を行うかの判定クラス
// 
//*****************************************
//--------------------------------
// 判定
//--------------------------------
bool OperateButtonSelectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Operate_Button) == TitleUI::SelectState::Select_Game_Start);
}

//********************************************
// 
// 操作ボタンの非選択時描画を行うかの判定クラス
// 
//********************************************
//--------------------------------
// 判定
//--------------------------------
bool OperateButtonUnselectDrawJudgment::Judgment()
{
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleUI* button_ui = dynamic_cast<TitleUI*>(ui.get());

	return (button_ui->GetButtonState(TitleUI::ButtonType::Operate_Button) == TitleUI::SelectState::Unselect_Game_Start);
}
