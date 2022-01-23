#include "UI.h"
#include "UIActionBase.h"
#include "UINodeTree.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
//UI::UI(const UIParameter& parameter)
//{
//	// パラメータ設定
//	SetParameter(parameter);
//}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
UI::~UI()
{
	delete ui_node_tree;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UI::Update(float elapsed_time)
{
	// 更新処理
	ui_node_tree->Run(elapsed_time);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UI::Render(ID3D11DeviceContext* context)
{
	ui_node_tree->Render(context);
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