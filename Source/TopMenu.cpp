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
	"操作",
	"タイトルに戻る"
};

//-----------------------------------------------
// コンストラクタ
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
// 更新処理
//-----------------------------------------------
void TopMenu::Update(float elapsed_time)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	// メニューを閉じる
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
			// TODO MENUOPENEVENTの書式どうする
			// TODO タイトルに戻る前に確認のポップアップを入れるか
			//Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
			// 直接タイトルへ戻る
			MessageData::MenuCloseData data;
			Messenger::Instance().SendData(MessageData::MENUCLOSEEVENT, &data);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle()));
		}
	}
}
// 描画処理
void TopMenu::Render(ID3D11DeviceContext* context)
{
	button_list->Render(context);
}
