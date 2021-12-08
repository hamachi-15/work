#include "UI.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Texture.h"
#include "ResourceManager.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
UI::UI(const UIParameter& parameter)
{
	// パラメータ設定
	SetParameter(parameter);
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
UI::~UI()
{
}

//-----------------------------------------------
// パラメータ設定
//-----------------------------------------------
void UI::SetParameter(const UIParameter& parameter)
{
	SetTexture(parameter.filename);
	if (parameter.parent != nullptr)
	{
		DirectX::XMFLOAT2 position;
		position.x = parameter.parent->position.x + parameter.position.x;
		position.y = parameter.parent->position.y + parameter.position.y;
		SetPosition(position);
	}
	else
	{
		SetPosition(parameter.position);
	}
	SetParent(parameter.parent);
	SetName(parameter.name);
	SetAngle(parameter.angle);
	SetSize({(float)GetTexture()->GetWidth(), (float)GetTexture()->GetHeight()});
}

//-----------------------------------------------
// テクスチャ設定
//-----------------------------------------------
void UI::SetTexture(const char* filename)
{
	// テクスチャ読み込み
	texture = ResourceManager::Instance().LoadTexture(filename);
}

//-----------------------------------------------
// ワールド座標からスクリーン座標に変換する
//-----------------------------------------------
DirectX::XMFLOAT2 UI::ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
	const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//// ビューポート
	//D3D11_VIEWPORT viewport;
	//UINT num_viewports = 1;
	//context->RSGetViewports(&num_viewports, &viewport);
	//float viewportX = viewport.TopLeftX;
	//float viewportY = viewport.TopLeftY;
	//float viewport_width = static_cast<float>(viewport.Width);
	//float viewport_hight = static_cast<float>(viewport.Height);
	//float viewportMinZ = viewport.MinDepth;
	//float viewportMaxZ = viewport.MaxDepth;

	//// 変換行列
	//DirectX::XMMATRIX mview = DirectX::XMLoadFloat4x4(&view);
	//DirectX::XMMATRIX mprojection = DirectX::XMLoadFloat4x4(&projection);
	//DirectX::XMMATRIX mworld = DirectX::XMMatrixIdentity();

	//// ワールド座標からスクリーン座標へ変換
	//DirectX::XMFLOAT3 enemy_pos = enemy->GetPosition();
	//enemy_pos.y += enemy->GetHeight();
	//DirectX::XMVECTOR v_enemy_pos = DirectX::XMLoadFloat3(&enemy_pos);


	//DirectX::XMVECTOR pos = DirectX::XMVector3Project(
	//	v_enemy_pos,
	//	viewportX, viewportY,
	//	viewport_width, viewport_hight,
	//	viewportMinZ, viewportMaxZ,
	//	mprojection, mview, mworld);
	//DirectX::XMFLOAT2 screen_pos;
	//DirectX::XMStoreFloat2(&screen_pos, pos);

	//return screen_pos;
	return { 0,0 };
}

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
	for (std::shared_ptr<UI> ui : remove)
	{
		std::vector<std::shared_ptr<UI>>::iterator remove = std::find(ui_list.begin(), ui_list.end(), ui);
		ui_list.erase(remove);
	}
	std::vector<std::shared_ptr<UI>>::iterator iterate_remove = remove.begin();
	for (; iterate_remove != remove.end(); iterate_remove = remove.begin())
	{
		remove.erase(iterate_remove);
	}

	// 更新処理
	for (std::shared_ptr<UI> ui : ui_list)
	{
		ui->Update(elapsed_time);
	}
}

//------------------------------
// 描画処理
//------------------------------
void UIManager::Draw(ID3D11DeviceContext* context)
{
	Graphics::Instance().GetSpriteShader()->Begin(context);
	Sprite sprite;
	for (std::shared_ptr<UI> ui : ui_list)
	{
		DirectX::XMFLOAT2 position = ui->GetPosition();
		DirectX::XMFLOAT2 size = ui->GetSize();
		Texture* texture = ui->GetTexture();
		sprite.Render(context, texture,
			position.x,position.y,
			size.x, size.y, 
			0, 0,
			size.x, size.y);
	}
	Graphics::Instance().GetSpriteShader()->End(context);
}

//------------------------------
// UI登録
//------------------------------
void UIManager::RegisterUI(std::shared_ptr<UI> ui)
{
	ui_list.emplace_back(ui);
}

//------------------------------
// UI削除
//------------------------------
void UIManager::UnRegisterUI(std::shared_ptr<UI> ui)
{
	remove.emplace_back(ui);

	// 親が設定されていれば行列に設定
	if (ui->GetParent()) 
	{
		remove.emplace_back(ui->GetParent());
	}
}

