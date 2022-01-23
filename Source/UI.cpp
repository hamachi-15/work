#include "UI.h"
#include "UIActionBase.h"
#include "UINodeTree.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
//UI::UI(const UIParameter& parameter)
//{
//	// �p�����[�^�ݒ�
//	SetParameter(parameter);
//}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
UI::~UI()
{
	delete ui_node_tree;
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void UI::Update(float elapsed_time)
{
	// �X�V����
	ui_node_tree->Run(elapsed_time);
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void UI::Render(ID3D11DeviceContext* context)
{
	ui_node_tree->Render(context);
}

//-----------------------------------------------
// ���[���h���W����X�N���[�����W�ɕϊ�����
//-----------------------------------------------
DirectX::XMFLOAT2 UI::ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
	const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//// �r���[�|�[�g
	//D3D11_VIEWPORT viewport;
	//UINT num_viewports = 1;
	//context->RSGetViewports(&num_viewports, &viewport);
	//float viewportX = viewport.TopLeftX;
	//float viewportY = viewport.TopLeftY;
	//float viewport_width = static_cast<float>(viewport.Width);
	//float viewport_hight = static_cast<float>(viewport.Height);
	//float viewportMinZ = viewport.MinDepth;
	//float viewportMaxZ = viewport.MaxDepth;

	//// �ϊ��s��
	//DirectX::XMMATRIX mview = DirectX::XMLoadFloat4x4(&view);
	//DirectX::XMMATRIX mprojection = DirectX::XMLoadFloat4x4(&projection);
	//DirectX::XMMATRIX mworld = DirectX::XMMatrixIdentity();

	//// ���[���h���W����X�N���[�����W�֕ϊ�
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