#include "UIManager.h"
#include "ShaderManager.h"
#include "Sprite.h"

#include "UI.h"

//--------------------------------
// コンストラクタ
//--------------------------------
UIManager::UIManager()
{
}

//--------------------------------
// デストラクタ
//--------------------------------
UIManager::~UIManager()
{
}

//------------------------------
// 更新処理
//------------------------------
void UIManager::Update(float elapsed_time)
{
	// 破棄行列に登録されていれば破棄させる
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
// 描画処理
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
// UI登録
//------------------------------
void UIManager::RegisterUI(std::shared_ptr<UI> ui)
{
	// UIを登録
	ui_starts.emplace_back(ui);
}

//------------------------------
// UI削除
//------------------------------
void UIManager::UnRegisterUI(std::shared_ptr<UI> ui)
{
	ui_removes.emplace_back(ui);
}
