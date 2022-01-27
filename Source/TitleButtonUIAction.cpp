#include "TitleButtonUIAction.h"
#include "TitleButtonUI.h"
#include "UIManager.h"
#include "SceneManager.h"

// ���s����
void TitleButtonUIAction::Action(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	std::shared_ptr<UI> ui = UIManager::Instance().GetUI("TitleButtonUI");
	TitleButtonUI* button_ui = dynamic_cast<TitleButtonUI*>(ui.get());
	
	Scene* scene = SceneManager::Instance().GetCurrentScene();

	if (!button_ui) return;

	if (gamepad.GetButtonDown() & GamePad::BTN_LEFT)
	{
		button_ui->SetSelectState(TitleButtonUI::ButtonType::Start_Button, TitleButtonUI::SelectState::Select_Game_Start);
		button_ui->SetSelectState(TitleButtonUI::ButtonType::Operate_Button, TitleButtonUI::SelectState::Unselect_Game_Start);
	}
	if (gamepad.GetButtonDown() & GamePad::BTN_RIGHT)
	{
		button_ui->SetSelectState(TitleButtonUI::ButtonType::Start_Button, TitleButtonUI::SelectState::Unselect_Game_Start);
		button_ui->SetSelectState(TitleButtonUI::ButtonType::Operate_Button, TitleButtonUI::SelectState::Select_Game_Start);
	}

	// A,B,X,Y�̂����ꂩ�̃{�^���������ꂽ�Ƃ��ɃQ�[���V�[���ɑJ�ڂ���
	if (button_ui->GetButtonState(TitleButtonUI::ButtonType::Start_Button) == TitleButtonUI::SelectState::Select_Game_Start &&
		gamepad.GetButtonDown() & any_button)
	{
		// �V�[���`�F���W�t���O��ݒ�
		scene->SetSceneChangeFlag();
	}
}
