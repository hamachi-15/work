#include "UI.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Texture.h"
#include "ResourceManager.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
UI::UI(const UIParameter& parameter)
{
	// �p�����[�^�ݒ�
	SetParameter(parameter);
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
UI::~UI()
{
}

//-----------------------------------------------
// �p�����[�^�ݒ�
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
// �e�N�X�`���ݒ�
//-----------------------------------------------
void UI::SetTexture(const char* filename)
{
	// �e�N�X�`���ǂݍ���
	texture = ResourceManager::Instance().LoadTexture(filename);
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

//--------------------------------
// �R���X�g���N�^
//--------------------------------
UIManager::UIManager()
{
}

//--------------------------------
// �f�X�g���N�^
//--------------------------------
UIManager::~UIManager()
{
}

//------------------------------
// �X�V����
//------------------------------
void UIManager::Update(float elapsed_time)
{
	// �j���s��ɓo�^����Ă���Δj��������
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

	// �X�V����
	for (std::shared_ptr<UI> ui : ui_list)
	{
		ui->Update(elapsed_time);
	}
}

//------------------------------
// �`�揈��
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
// UI�o�^
//------------------------------
void UIManager::RegisterUI(std::shared_ptr<UI> ui)
{
	ui_list.emplace_back(ui);
}

//------------------------------
// UI�폜
//------------------------------
void UIManager::UnRegisterUI(std::shared_ptr<UI> ui)
{
	remove.emplace_back(ui);

	// �e���ݒ肳��Ă���΍s��ɐݒ�
	if (ui->GetParent()) 
	{
		remove.emplace_back(ui->GetParent());
	}
}

