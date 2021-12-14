#include <imgui.h>
#include "SceneBattle.h"
#include "Graphics.h"
#include "Camera.h"
#include "CameraController.h"
#include "Template.h"
#include "Light.h"
#include "Input.h"
#include "Movement.h"
#include "Actor.h"
#include "ActorManager.h"

#include "PhongVarianceShadowMap.h"
#include "GaussianBlurShader.h"
#include "ReflectSeaShader.h"
#include "UseCubeMapShader.h"
#include "LambertShader.h"
#include "BloomShader.h"

#include "EnemyManager.h"
#include "Player.h"
#include "EnemySlime.h"
#include "Stage.h"
#include "MenuSystem.h"
#include "Messenger.h"
#include "MessageData.h"
#include "Collision.h"
#include "Texture.h"
#include "Sprite.h"
#include "GameDataBase.h"
#include "MetaAI.h"

#include "UI.h"
SceneBattle::SceneBattle()
{
}

SceneBattle::~SceneBattle()
{
}

void SceneBattle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	// �V�[�����ݒ�
	SetName("SceneBattle");

	// �f�[�^�x�[�X������
	GameDataBase::Instance();

	// ���C�g������
	Light::Initialize();

	// �J���������ݒ�
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(-400, 16, -416),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		800.0f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();
	camera_controller->SetCameraAngle({ DirectX::XMConvertToRadians(25), 0.0f, 0.0f });
	//camera_controller->SetTarget(DirectX::XMFLOAT3(0, , -416));

	// �V�F�[�_�[������
	bloom = std::make_unique<Bloom>(device);

	// �e�N�X�`���̓ǂݍ���
	sprite = std::make_unique<Sprite>();
	sky = std::make_unique<Texture>();
	sky->Load("Data/Sprite/SkyBox/FS002_Night.png");

	// �X�e�[�W�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/Tile.mdl");
		actor->SetName("Stage");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, DirectX::XMConvertToRadians(-90), 0));
		actor->SetScale(DirectX::XMFLOAT3(5.f, 1.f, 5.f));
		actor->AddComponent<Stage>();
		actor->AddShader<PhongVarianceShadowMap>(Graphics::Instance().GetDevice());
	}

	// �v���C���[�ǂݍ���
	{
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetUpModel("Data/Model/RPG-Character/Swordman.mdl");
		actor->SetName("Player");
		actor->SetAnimationNodeName("Motion");
		//actor->SetPosition(DirectX::XMFLOAT3(-100, 16, -116));
		actor->SetPosition(DirectX::XMFLOAT3(0, 1, 0));
		actor->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetScale(DirectX::XMFLOAT3(0.04f, 0.04f, 0.04f));
		actor->AddComponent<Movement>();
		actor->AddComponent<Charactor>();
		actor->AddComponent<Player>();
		actor->AddShader<PhongVarianceShadowMap>(Graphics::Instance().GetDevice());
	}
	ActorManager::Instance().Update(0.01f);
	ActorManager::Instance().UpdateTransform();

	// �X�N���v�g����G�̐���
	EnemyManager::Instance().CreateEnemyScriptData();
}

void SceneBattle::Finalize()
{
	// �G�}�l�[�W���[�̃N���A
	EnemyManager::Instance().AllRemove();

	// �A�N�^�[�̔j��
	ActorManager::Instance().AllDestroy();

	// �R���W�����̔j��
	CollisionManager::Instance().Destroy();

	// ���b�Z���W���[�̃N���A
	Messenger::Instance().Clear();
}

void SceneBattle::Update(float elapsed_time)
{
	//	���j���[�I�[�v�����̓��j���[���X�V����
	MenuSystem::Instance().Update(elapsed_time);

	//	���j���[�I�[�v�����ł���΃Q�[�����̍X�V���s��Ȃ�
	if (MenuSystem::Instance().IsOpened())
	{
		return;
	}
	else
	{
		//	���j���[�I�[�v��
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_START)
		{
			MessageData::MenuOpenData data;
			data.menu_id = MenuId::TopMenu;
			Messenger::Instance().SendData(MessageData::MENUOPENEVENT, &data);
		}
	}

	//���C�g
	static float light_angle = DirectX::XM_PI;
	if (GetKeyState('E') < 0) light_angle += elapsed_time * 2.0f;
	if (GetKeyState('Q') < 0) light_angle -= elapsed_time * 2.0f;

	Light::SetAmbient(DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f));
	//���C�g����
	LightDir.x = sinf(light_angle);
	LightDir.y = -1.0f;
	LightDir.z = cosf(light_angle);
	Light::SetDirLight(LightDir, DirectX::XMFLOAT3(0.6f, 0.6f, 0.6f));

	// ���^AI�̍X�V����
	MetaAI::Instance().Update(elapsed_time);

	// �A�N�^�[�X�V����
	ActorManager::Instance().Update(elapsed_time);
	ActorManager::Instance().UpdateTransform();

	// �����蔻��X�V����
	CollisionManager::Instance().Update();

	// UI�X�V����
	UIManager::Instance().Update(elapsed_time);

	// �J�����X�V����
	std::shared_ptr<Actor> actor = ActorManager::Instance().GetActor("Player");
	camera_controller->SetTarget({
		actor->GetPosition().x,
		actor->GetPosition().y + 1.0f,
		actor->GetPosition().z });
	camera_controller->Update(elapsed_time);
}

void SceneBattle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	// �X�N���[���T�C�Y�擾
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };

	// �`�揈��
	render_context.light_direction = Light::LightDir;
	render_context.ShadowParameter = { shadow_color.x, shadow_color.y, shadow_color.z, 0.000001f };

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// �o�b�N�o�b�t�@�̃N���A����
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		// ��ʃN���A
		graphics.ScreenClear(&render_target_view, depth_stencil_view);
	}

	// �X�N���[���e�N�X�`���ɕ`��
	ScreenRender(context, render_context, screen_size);

	// �|�X�g�e�N�X�`���`��
	PostRender(context, render_context, screen_size);

	// �o�b�N�o�b�t�@�ɕ`��
	BuckBufferRender(context, render_context, screen_size);

	// GUI�`��
	//OnGui();
}


//-------------------------------------
// �X�N���[���e�N�X�`���`��
//-------------------------------------
void SceneBattle::ScreenRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();

	// �X�N���[���e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ肵�ĉ�ʃN���A
	ID3D11RenderTargetView* screen_texture = graphics.GetTexture()->GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
	{
		graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);
		graphics.ScreenClear(&screen_texture, depth_stencil_view);
	}
	// �r���[�|�[�g�����ɖ߂�
	graphics.SetViewport(screen_size.x, screen_size.y);

	// �X�J�C�{�b�N�X�`��
	{
		graphics.GetSkyBoxShader()->Begin(context, render_context);
		sprite->Render(context,
			sky.get(),
			0, 0,
			screen_size.x, screen_size.y,
			0, 0,
			static_cast<float>(sky->GetWidth()), static_cast<float>(sky->GetHeight()),
			0,
			1, 1, 1, 1);
		graphics.GetSkyBoxShader()->End(context);
	}

	// �f�o�b�O�v���~�e�B�u�`��
	{
		EnemyManager::Instance().DrawDebugPrimitive();
		CollisionManager::Instance().Draw();
		graphics.GetDebugRenderer()->Render(context, render_context.view, render_context.projection);
	}

	// �A�N�^�[�`��
	{
		// �V���h�E�}�b�v�쐬
		//ActorManager::Instance().ShadowRender(render_context, blur_render_context);

		// �����_�[�^�[�Q�b�g�̉�
		//graphics.SetRenderTargetView(&screen_texture, depth_stencil_view);

		// �r���[�|�[�g�̐ݒ�
		//graphics.SetViewport(graphics.GetScreenWidth(), graphics.GetScreenHeight());

		// �`��
		ActorManager::Instance().Render(render_context);
	}
}

//-------------------------------------
// �|�X�g�e�N�X�`���`��
//-------------------------------------
void SceneBattle::PostRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	bloom_texture = bloom->Render(context, render_context);

	//Texture* texture = bulr->Render(graphics.GetTexture());
	//{
	//	ID3D11RenderTargetView* render_target_view[1] = { bulr_texture->GetRenderTargetView() };
	//	graphics.SetRenderTargetView(render_target_view, depth_stencil_view);
	//}
	//// �r���[�|�[�g�̐ݒ�
	//graphics.SetViewport(screen_width, screen_height);
	//graphics.GetSpriteShader()->Begin(context);
	//sprite->Render(context,
	//	texture,
	//	0, 0,
	//	screen_size.x, screen_size.y,
	//	0, 0,
	//	texture->GetWidth(), texture->GetHeight());
	//graphics.GetSpriteShader()->End(context);
}

//-------------------------------------
//�o�b�N�o�b�t�@�`��
//-------------------------------------
void SceneBattle::BuckBufferRender(ID3D11DeviceContext* context, RenderContext& render_context, const DirectX::XMFLOAT2& screen_size)
{
	Graphics& graphics = Graphics::Instance();

	// �����_�[�^�[�Q�b�g�ݒ�
	{
		ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();
		graphics.SetRenderTargetView(&render_target_view, depth_stencil_view);
	}
	// �r���[�|�[�g�ݒ�
	graphics.SetViewport(screen_size.x, screen_size.y);

	// �o�b�N�o�b�t�@�ɃX�N���[���e�N�X�`����`��
	graphics.GetSpriteShader()->Begin(context);
	// �X�N���[���e�N�X�`��
	sprite->Render(context, graphics.GetTexture(),
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)graphics.GetTexture()->GetWidth(), (float)graphics.GetTexture()->GetHeight(),
		0,
		1, 1, 1, 1);
	// �P�x���o�e�N�X�`�������Z����
	sprite->AddRender(context,
		bloom_texture,
		0, 0,
		screen_size.x, screen_size.y,
		0, 0,
		(float)bloom_texture->GetWidth(), (float)bloom_texture->GetHeight());
	graphics.GetSpriteShader()->End(context);

	// ���j���[�`��
	{
		if (MenuSystem::Instance().IsOpened())
		{
			graphics.GetSpriteShader()->Begin(context);
			MenuSystem::Instance().Render(context);
			graphics.GetSpriteShader()->End(context);
		}
	}

	// UI�`�揈��
	UIManager::Instance().Draw(context);

	// �V���h�E�}�b�v
	//graphics.GetSpriteShader()->Begin(context);
	//for (int i = 0; i < 3; ++i)
	//{
	//	sprite->Render(context,
	//		ActorManager::Instance().GetShadowTexture(i),
	//		0 + 200 * (i), 0,
	//		200, 200,
	//		0, 0,
	//		(float)ActorManager::Instance().GetShadowTexture(i)->GetWidth(), (float)ActorManager::Instance().GetShadowTexture(i)->GetHeight(),
	//		0,
	//		1, 1, 1, 1);
	//}
	//graphics.GetSpriteShader()->End(context);

}

//-------------------------------------
// GUI�`��
//-------------------------------------
//void SceneBattle::OnGui()
//{
//	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
//
//	ImGui::Begin("Shader", nullptr, ImGuiWindowFlags_None);
//	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------�V���h�E�}�b�v�p�u���[-------");
//	ImGui::SliderInt("KarnelSize", &blur_render_context.karnel_size, 1, 15);
//
//	ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"-------�V���h�E�}�b�v-------");
//	ImGui::ColorEdit3("ShadowColor", (float*)&shadow_color);
//	ImGui::End();
//
//	const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
//	static int listbox_item_current = 1;
//	ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
//
//	static float f1 = 1.00f, f2 = 0.0067f;
//	ImGui::DragFloat("drag float", &f1, 0.005f);
//	static int e = 0;
//	ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
//	ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
//	ImGui::RadioButton("radio c", &e, 2);
//
//	static bool selected[4] = { false, true, false, false };
//	ImGui::Selectable("1. I am selectable", &selected[0]);
//	ImGui::Selectable("2. I am selectable", &selected[1]);
//	ImGui::Text("3. I am not selectable");
//	ImGui::Selectable("4. I am selectable", &selected[2]);
//	if (ImGui::Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
//		if (ImGui::IsMouseDoubleClicked(0))
//			selected[3] = !selected[3];
//
//
//	//bool* p_open = (bool*)1;
//	//ImGui::SetNextWindowPos(ImVec2(10, 10));
//	//if (!ImGui::Begin("Example: Fixed Overlay", p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
//	//{
//	//	ImGui::End();
//	//	return;
//	//}
//	//ImGui::Text("Simple overlay\non the top-left side of the screen.");
//	//ImGui::Separator();
//	//ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
//	//ImGui::End();
//
//	//static int i1 = 0;
//	//ImGui::SliderInt("slider int", &i1, -1, 3);
//	//ImGui::SameLine();
//
//	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
//
//	if (ImGui::TreeNode("Borders"))
//	{
//		ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
//		if (ImGui::TreeNode("Border"))
//			ImGui::TreePop();
//		// NB: Future columns API should allow automatic horizontal borders.
//		static bool h_borders = true;
//		static bool v_borders = true;
//		static int columns_count = 4;
//		const int lines_count = 3;
//		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
//		ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
//		if (columns_count < 2)
//			columns_count = 2;
//		ImGui::SameLine();
//		ImGui::Checkbox("horizontal", &h_borders);
//		ImGui::SameLine();
//		ImGui::Checkbox("vertical", &v_borders);
//		ImGui::Columns(columns_count, NULL, v_borders);
//		for (int i = 0; i < columns_count * lines_count; i++)
//		{
//			if (h_borders && ImGui::GetColumnIndex() == 0)
//				ImGui::Separator();
//			ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
//			ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
//			ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
//			ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
//			ImGui::Text("Long text that is likely to clip");
//			ImGui::RadioButton("radio c", &e, 2);
//			ImGui::Checkbox("vertical", &v_borders);
//			ImGui::NextColumn();
//		}
//		ImGui::Columns(1);
//		if (h_borders)
//			ImGui::Separator();
//		ImGui::TreePop();
//	}
//	if (ImGui::TreeNode("Mixed items"))
//	{
//		ImGui::Columns(3, "mixed");
//		ImGui::Separator();
//
//		ImGui::Text("Hello");
//		ImGui::Button("Banana");
//		ImGui::NextColumn();
//
//		ImGui::Text("ImGui");
//		ImGui::Button("Apple");
//		static float foo = 1.0f;
//		ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
//		ImGui::Text("An extra line here.");
//		ImGui::NextColumn();
//
//		ImGui::Text("Sailor");
//		ImGui::Button("Corniflower");
//		static float bar = 1.0f;
//		ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
//		ImGui::NextColumn();
//
//		if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
//		if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
//		if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
//		ImGui::Columns(1);
//		ImGui::Separator();
//		ImGui::TreePop();
//	}
//	//if (ImGui::TreeNode("Word-wrapping"))
//	//{
//	//	ImGui::Columns(2, "word-wrapping");
//	//	ImGui::Separator();
//	//	ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
//	//	ImGui::TextWrapped("Hello Left");
//	//	ImGui::NextColumn();
//	//	ImGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
//	//	ImGui::TextWrapped("Hello Right");
//	//	ImGui::Columns(1);
//	//	ImGui::Separator();
//	//	ImGui::TreePop();
//	//}
//	//if (ImGui::CollapsingHeader("Filtering"))
//	//{
//	//	// Helper class to easy setup a text filter.
//	//	// You may want to implement a more feature-full filtering scheme in your own application.
//	//	static ImGuiTextFilter filter;
//	//	ImGui::Text("Filter usage:\n"
//	//		"  \"\"         display all lines\n"
//	//		"  \"xxx\"      display lines containing \"xxx\"\n"
//	//		"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
//	//		"  \"-xxx\"     hide lines containing \"xxx\"");
//	//	filter.Draw();
//	//	const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
//	//	for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
//	//		if (filter.PassFilter(lines[i]))
//	//			ImGui::BulletText("%s", lines[i]);
//	//}
//	//if (ImGui::TreeNode("Tabbing"))
//	//{
//	//	ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
//	//	static char buf[32] = "hello";
//	//	ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
//	//	ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
//	//	ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
//	//	ImGui::PushAllowKeyboardFocus(false);
//	//	ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
//	//	//ImGui::SameLine(); HelpMarker("Use ImGui::PushAllowKeyboardFocus(bool) to disable tabbing through certain widgets.");
//	//	ImGui::PopAllowKeyboardFocus();
//	//	ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
//	//	ImGui::TreePop();
//	//}
//	//if (ImGui::TreeNode("Focus from code"))
//	//{
//	//	bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
//	//	bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
//	//	bool focus_3 = ImGui::Button("Focus on 3");
//	//	int has_focus = 0;
//	//	static char buf[128] = "click on a button to set focus";
//	//	if (focus_1) ImGui::SetKeyboardFocusHere();
//	//	ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
//	//	if (ImGui::IsItemActive()) has_focus = 1;
//	//	if (focus_2) ImGui::SetKeyboardFocusHere();
//	//	ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
//	//	if (ImGui::IsItemActive()) has_focus = 2;
//	//	ImGui::PushAllowKeyboardFocus(false);
//	//	if (focus_3) ImGui::SetKeyboardFocusHere();
//	//	ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
//	//	if (ImGui::IsItemActive()) has_focus = 3;
//	//	ImGui::PopAllowKeyboardFocus();
//	//	if (has_focus)
//	//		ImGui::Text("Item with focus: %d", has_focus);
//	//	else
//	//		ImGui::Text("Item with focus: <none>");
//	//	// Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
//	//	static float f3[3] = { 0.0f, 0.0f, 0.0f };
//	//	int focus_ahead = -1;
//	//	if (ImGui::Button("Focus on X")) { focus_ahead = 0; } ImGui::SameLine();
//	//	if (ImGui::Button("Focus on Y")) { focus_ahead = 1; } ImGui::SameLine();
//	//	if (ImGui::Button("Focus on Z")) { focus_ahead = 2; }
//	//	if (focus_ahead != -1) ImGui::SetKeyboardFocusHere(focus_ahead);
//	//	ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);
//
//	//	ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
//	//	ImGui::TreePop();
//	//}
//	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.4f);
//	ImGui::GetStyle().Colors[ImGuiCol_Border].x = 1.0f;
//	ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 1.0f, 1.0f, 0.7f);
//	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
//	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 0.7f);
//}
