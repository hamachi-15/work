#include "TopMenu.h"
#include "Input.h"
#include "MessageData.h"
#include "Messenger.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
enum	Command
{
	OperateDictionary,
	TitleBackDictionary,

};
static	const	char* command_name[] =
{
	"����",
	"�^�C�g���ɖ߂�"
};

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
TopMenu::TopMenu()
{
	button_list = std::make_unique<ButtonList>();

	button_list->SetPosition({ 50, 50 });
	button_list->SetSize({ 250, 50 });
	for (auto& it : command_name)
	{
		button_list->Register(it);
	}
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void TopMenu::Update(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	// ���j���[�����
	if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		MessageData::MenuCloseData data;
		// TODO
		Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
		return;
	}

	button_list->Update(elapsed_time);

	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		if (button_list->GetSelectedButton() == command_name[Command::OperateDictionary])
		{
			MessageData::MenuOpenData data;
			data.menu_id = MenuId::OperateDictionaryMenu;
			// TODO
			Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
		}
		if (button_list->GetSelectedButton() == command_name[Command::TitleBackDictionary])
		{
			//MessageData::MenuOpenData data;
			//data.menu_id = MenuId::TitleBackDictionaryMenu;
			// TODO MENUOPENEVENT�̏����ǂ�����
			// TODO �^�C�g���ɖ߂�O�Ɋm�F�̃|�b�v�A�b�v�����邩
			//Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
			// ���ڃ^�C�g���֖߂�
			MessageData::MenuCloseData data;
			Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
		}
	}
}
// �`�揈��
void TopMenu::Render(ID3D11DeviceContext* context)
{
	button_list->Render(context);
}
