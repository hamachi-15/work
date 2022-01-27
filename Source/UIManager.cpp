#include "UIManager.h"
#include "ShaderManager.h"
#include "Sprite.h"

#include "UI.h"

//--------------------------------
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//--------------------------------
UIManager::UIManager()
{
}

//--------------------------------
// ƒfƒXƒgƒ‰ƒNƒ^
//--------------------------------
UIManager::~UIManager()
{
	// ‘S”jŠü
	AllDelete();
}

//------------------------------
// XVˆ—
//------------------------------
void UIManager::Update(float elapsed_time)
{
	// ”jŠüs—ñ‚É“o˜^‚³‚ê‚Ä‚¢‚ê‚Î”jŠü‚³‚¹‚é
	for (std::shared_ptr<UI> ui : ui_removes)
	{
		std::vector<std::shared_ptr<UI>>::iterator remove = std::find(ui_updates.begin(), ui_updates.end(), ui);
		ui_updates.erase(remove);
	}
	std::vector<std::shared_ptr<UI>>::iterator iterate_remove = ui_removes.begin();
	for (; iterate_remove != ui_removes.end(); iterate_remove = ui_removes.begin())
	{
		ui_removes.erase(iterate_remove);
	}

	for (std::shared_ptr<UI> ui : ui_starts)
	{
		ui->Start();
		ui_updates.emplace_back(ui);
	}
	ui_starts.clear();

	for (std::shared_ptr<UI> ui : ui_updates)
	{
		ui->Update(elapsed_time);
	}
}

//------------------------------
// •`‰æˆ—
//------------------------------
void UIManager::Draw(ID3D11DeviceContext* context)
{
	ShaderManager& shader_manager = ShaderManager::Instance();
	std::shared_ptr<Shader> sprite_shader = shader_manager.GetShader(ShaderManager::ShaderType::Sprite);

	sprite_shader->Begin(context);
	Sprite sprite;
	for (std::shared_ptr<UI> ui : ui_updates)
	{
		ui->Render(context);
	}
	sprite_shader->End(context);
}

//------------------------------
// UI“o˜^
//------------------------------
void UIManager::RegisterUI(std::shared_ptr<UI> ui)
{
	// UI‚ğ“o˜^
	ui_starts.emplace_back(ui);
}

//------------------------------
// UIíœ
//------------------------------
void UIManager::UnRegisterUI(std::shared_ptr<UI> ui)
{
	ui_removes.emplace_back(ui);
}

//------------------------------
// ‘Síœ
//------------------------------
void UIManager::AllDelete()
{
	std::vector<std::shared_ptr<UI>>::iterator iterate_update = ui_updates.begin();
	for (; iterate_update != ui_updates.end(); iterate_update = ui_updates.begin())
	{
		ui_updates.erase(iterate_update);
	}
	ui_updates.clear();
}

//------------------------------
// UIæ“¾
//------------------------------
std::shared_ptr<UI> UIManager::GetUI(std::string name)
{
	for (std::shared_ptr<UI> ui : ui_updates)
	{
		if (ui->GetName() == name)
		{
			return ui;
		}
	}

	return nullptr;
}
