#include <imgui.h>
#include "Graphics.h"
#include "ActorManager.h"
#include "PhongVarianceShadowMap.h"
#include "VarianceShadowMap.h"
#include "GaussianBlurShader.h"
#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"
#include "Misc.h"
#include "Light.h"
#include "GaussianXBlur.h"
#include "GaussianYBlur.h"
//------------------------------
// �R���X�g���N�^
//------------------------------
ActorManager::ActorManager()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	shadowmap = std::make_unique<VarianceShadowMap>(device);
	bulr = std::make_unique<GaussianBlur>(device);
	shader = std::make_unique<PhongVarianceShadowMap>(device);

	shadow_texture = std::make_unique<Texture>();
	shadow_texture->Create(BUFFER_WIDTH, BUFFER_HEIGHT, DXGI_FORMAT_R32G32_FLOAT);
	shadow_vsm_texture = std::make_unique<Texture>();
	shadow_vsm_texture->Create(BUFFER_WIDTH, BUFFER_HEIGHT, DXGI_FORMAT_R32G32_FLOAT);
	depth_texture = std::make_unique<Texture>();
	depth_texture->CreateDepthStencil(BUFFER_WIDTH, BUFFER_HEIGHT);

}

//------------------------------
// �f�X�g���N�^
//------------------------------
ActorManager::~ActorManager()
{
}

//------------------------------
// �쐬����
//------------------------------
std::shared_ptr<Actor> ActorManager::Create()
{
	std::shared_ptr<Actor> actor = std::make_unique<Actor>();
	{
		static int id = 0;
		char name[256];
		::sprintf_s(name, sizeof(name), "Actor%d", id++);
		actor->SetName(name);
	}
	start_actors.emplace_back(actor);
	return actor;
}

//------------------------------
// �j���z��ɒǉ�
//------------------------------
void ActorManager::Remove(std::shared_ptr<Actor> actor)
{	
	remove_actors.emplace_back(actor);
}

//------------------------------
// �j������
//------------------------------
void ActorManager::Destroy(std::shared_ptr<Actor> actor)
{
	std::vector<std::shared_ptr<Actor>>::iterator iterate_start = std::find(start_actors.begin(), start_actors.end(), actor);
	if (iterate_start != start_actors.end())
	{
		start_actors.erase(iterate_start);
	}
	std::vector<std::shared_ptr<Actor>>::iterator iterate_update = std::find(update_actors.begin(), update_actors.end(), actor);
	if (iterate_update != update_actors.end())
	{
		update_actors.erase(iterate_update);
	}
	std::set<std::shared_ptr<Actor>>::iterator iterate_selection = selection_actors.find(actor);
	if (iterate_selection != selection_actors.end())
	{
		selection_actors.erase(iterate_selection);
	}
}
void ActorManager::AllDestroy()
{
	std::vector<std::shared_ptr<Actor>>::iterator iterate_start = start_actors.begin();
	for(;iterate_start != start_actors.end(); iterate_start = start_actors.begin())
	{
		start_actors.erase(iterate_start);
	}
	std::vector<std::shared_ptr<Actor>>::iterator iterate_update = update_actors.begin();
	for (; iterate_update != update_actors.end(); iterate_update = update_actors.begin())
	{
		update_actors.erase(iterate_update);
	}
	std::set<std::shared_ptr<Actor>>::iterator iterate_selection = selection_actors.begin();
	for (; iterate_selection != selection_actors.end(); iterate_selection = selection_actors.begin())
	{
		selection_actors.erase(iterate_selection);
	}
	std::vector<std::shared_ptr<Actor>>::iterator iterate_remove = remove_actors.begin();
	for (; iterate_remove != remove_actors.end(); iterate_remove = remove_actors.begin())
	{
		remove_actors.erase(iterate_remove);
	}
	shader_name = "";
}


//------------------------------
// �X�V����
//------------------------------
void ActorManager::Update(float elapsed_time)
{
	for (std::shared_ptr<Actor>& actor : start_actors)
	{
		actor->Start();
		update_actors.emplace_back(actor);
	}
	start_actors.clear();

	for (std::shared_ptr<Actor>& actor : update_actors)
	{
		actor->Update(elapsed_time);
	}

	// �j���z��ɃA�N�^�[�������Ă���Ƃ�update_actors�z�񂩂炻�̃A�N�^�[��j��������
	for (std::shared_ptr<Actor> actor : remove_actors)
	{
		std::vector<std::shared_ptr<Actor>>::iterator remove = std::find(update_actors.begin(), update_actors.end(), actor);
		update_actors.erase(remove);
	}
	std::vector<std::shared_ptr<Actor>>::iterator iterate_remove = remove_actors.begin();
	for (; iterate_remove != remove_actors.end(); iterate_remove = remove_actors.begin())
	{
		remove_actors.erase(iterate_remove);
	}
}

//------------------------------
// �s��X�V����
//------------------------------
void ActorManager::UpdateTransform()
{
	for (std::shared_ptr<Actor>& actor : update_actors)
	{
		actor->UpdateTransform();
	}
}

//------------------------------
// �V���h�E�}�b�v�`�揈��
//------------------------------
void ActorManager::ShadowRender(RenderContext& render_context, BlurRenderContext& bulr_render_context)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	std::shared_ptr<Actor> actor = GetActor("Player");
	{
		ID3D11RenderTargetView* rtv[1] = { shadow_texture->GetRenderTargetView() };
		ID3D11DepthStencilView* dsv = depth_texture->GetDepthStencilView();
		context->OMSetRenderTargets(1, rtv, dsv);
	
		// ��ʃN���A
		float clear_color[4] = { 1.0f,1.0f,1.0f,1.0f };
		context->ClearRenderTargetView(rtv[0], clear_color);
		context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)BUFFER_WIDTH;
		vp.Height = (FLOAT)BUFFER_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);

		// ���C�g�ʒu����̃r���[�ݒ�
		DirectX::XMFLOAT3 t = { 0, 0, 0 };
		DirectX::XMFLOAT3 pos, target, up;
		DirectX::XMFLOAT4X4 view, projection;
		target = actor->GetPosition();
		pos.x = target.x - Light::LightDir.x * 30.0f;
		pos.y = target.y - Light::LightDir.y * 30.0f;
		pos.z = target.z - Light::LightDir.z * 30.0f;
		//target = { 0, 0, 0 };
		up = { 0, 1, 0 };
		DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&pos);
		DirectX::XMVECTOR vtarget = DirectX::XMLoadFloat3(&target);
		DirectX::XMVECTOR vup = DirectX::XMLoadFloat3(&up);

		DirectX::XMMATRIX matview = DirectX::XMMatrixLookAtLH(eye, vtarget, vup);
		DirectX::XMStoreFloat4x4(&view, matview);

		DirectX::XMMATRIX pm = DirectX::XMMatrixIdentity();
		pm = DirectX::XMMatrixOrthographicLH(50.0f, 50.0f, 1.0f, 1000.0f);
		//pm = DirectX::XMMatrixOrthographicLH(10.0f, 10.0f, 0.1f, 200.0f);
		//pm = DirectX::XMMatrixOrthographicLH(320, 180, 1.0f, 100.0f);
		//pm = DirectX::XMMatrixOrthographicLH(20.0f, 20.0f, 0.1f, 200.0f);
		DirectX::XMStoreFloat4x4(&projection, pm);
		// TODO �ύX�����J�����̃r���[�ƃv���W�F�N�V�����̐ݒ�
		DirectX::XMStoreFloat4x4(&render_context.light_view_projection, matview * pm);
		
		shadowmap->Begin(context, render_context);
		for (std::shared_ptr<Actor>& actor : update_actors)
		{
			// ���f��������Ε`��
			Model* model = actor->GetModel();
			if (model != nullptr)
			{
				shadowmap->Draw(context, model);
			}
		}
		shadowmap->End(context);
	}

	// �u���[���|����
	{
		float blurx_texture_width = static_cast<float>(bulr->GetGaussianXBlurShader()->GetBlurXTexture()->GetWidth());
		float blurx_texture_height = static_cast<float>(bulr->GetGaussianXBlurShader()->GetBlurXTexture()->GetHeight());
		float blury_texture_width = static_cast<float>(bulr->GetGaussianYBlurShader()->GetBlurYTexture()->GetWidth());
		float blury_texture_height = static_cast<float>(bulr->GetGaussianYBlurShader()->GetBlurYTexture()->GetHeight());
		Sprite sprite;
		// X�����Ƀu���[���|����
		bulr->Begin(context, bulr_render_context, BlurType::XBlur);
		sprite.Render(context, shadow_texture.get(),
			0, 0,
			blurx_texture_width, blurx_texture_height,
			0, 0,
			(float)shadow_texture->GetWidth(), (float)shadow_texture->GetHeight(),
			0,
			1, 1, 1, 1);
		bulr->End(context, BlurType::XBlur);

		// Y�����Ƀu���[���|����
		bulr->Begin(context, bulr_render_context, BlurType::YBlur);
		sprite.Render(context, bulr->GetGaussianXBlurShader()->GetBlurXTexture(),
			0, 0,
			(float)bulr->GetGaussianYBlurShader()->GetBlurYTexture()->GetWidth(), (float)bulr->GetGaussianYBlurShader()->GetBlurYTexture()->GetHeight(),
			0, 0,
			blurx_texture_width, blurx_texture_height,
			0,
			1, 1, 1, 1);
		bulr->End(context, BlurType::YBlur);
	
		// �|�����u���[���V���h�E�}�b�v�ăe�N�X�`���ɕ`��
		// �����_�[�^�[�Q�b�g���V���h�E�}�b�v�ɐݒ�
		ID3D11RenderTargetView* rtv[1] = { shadow_vsm_texture->GetRenderTargetView() };
		ID3D11DepthStencilView* dsv = depth_texture->GetDepthStencilView();
		context->OMSetRenderTargets(1, rtv, dsv);

		// ��ʃN���A
		float clear_color[4] = { 1.0f,1.0f,1.0f,1.0f };
		context->ClearRenderTargetView(rtv[0], clear_color);
		context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)shadow_vsm_texture->GetWidth();
		vp.Height = (FLOAT)shadow_vsm_texture->GetHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		context->RSSetViewports(1, &vp);
		float screen_height = graphics.GetScreenHeight();

		graphics.GetSpriteShader()->Begin(context);
		sprite.Render(context,
			bulr->GetGaussianYBlurShader()->GetBlurYTexture(),
			0, 0,
			shadow_vsm_texture->GetWidth(), shadow_vsm_texture->GetHeight(),
			0, 0,
			blury_texture_width, blury_texture_height,
			0,
			1, 1, 1, 1);
		graphics.GetSpriteShader()->End(context);
		// �����_�[�^�[�Q�b�g�̉�
	ID3D11RenderTargetView* backbuffer = graphics.GetRenderTargetView();
	context->OMSetRenderTargets(1, &backbuffer, graphics.GetDepthStencilView());

	// �r���[�|�[�g�����ɖ߂�
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(graphics.GetScreenWidth());
	viewport.Height = static_cast<float>(graphics.GetScreenHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	graphics.GetSpriteShader()->Begin(context);
	sprite.Render(context,
		bulr->GetGaussianYBlurShader()->GetBlurYTexture(),
		100, 0,
		200, 200,
		0, 0,
		blury_texture_width, blury_texture_height,
		0,
		1, 1, 1, 1);
	graphics.GetSpriteShader()->End(context);

}
}

//------------------------------
// �L���[�u�}�b�v�`��
//------------------------------
void ActorManager::RenderCubeMap(RenderContext& render_context)
{
}

//------------------------------
// �`�揈��
//------------------------------
void ActorManager::Render(RenderContext& render_context)
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	shader->Begin(context, render_context);
	for (std::shared_ptr<Actor>& actor : update_actors)
	{
		// ���݃Z�b�g����Ă���V�F�[�_�[�Ƃ��ꂩ��̕`��Ɏg���V�F�[�_�[��������
		//if(strcmp(shader_name.c_str(),actor->GetShader()->GetShaderName()) != 0)
		//{	
		//	// �V�F�[�_�[�̏I������
		//	if(shader != nullptr)
		//		shader->End(context);
		//	// ���݂̃V�F�[�_�[�����ւ���
		//	shader = actor->GetShader();
		//	shader_name = shader->GetShaderName();
		//}
		// �V�F�[�_�[�Ƀ|�C���^�������Ă��Ȃ���΃A�T�[�g
		//_ASSERT_EXPR_A(shader, "shader is nullptr");

		// ���f��������Ε`��
		Model* model = actor->GetModel();
		if (model != nullptr)
		{
			shader->Draw(context, model);
		}
	}
	shader->End(context);
	// ���X�^�[�`��
	DrawLister();
	// �ڍו`��
	DrawDetail();
}

//------------------------------
// ���X�^�[�`��
//------------------------------
void ActorManager::DrawLister()
{
	ImGui::SetNextWindowPos(ImVec2(200, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	hidden_lister = !ImGui::Begin("Actor Lister", nullptr, ImGuiWindowFlags_None);
	if (!hidden_lister)
	{

		for (std::shared_ptr<Actor>& actor : update_actors)
		{
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
			if (selection_actors.find(actor) != selection_actors.end())
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::TreeNodeEx(actor.get(), node_flags, actor->GetName());
			if (ImGui::IsItemClicked())
			{
				// �P��I���̂ݑΉ�
				ImGuiIO& io = ImGui::GetIO();
				selection_actors.clear();
				selection_actors.insert(actor);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

//------------------------------
// �ڍו`��
//------------------------------
void ActorManager::DrawDetail()
{
	ImGui::SetNextWindowPos(ImVec2(350, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	hidden_detail = !ImGui::Begin("Actor Detail", nullptr, ImGuiWindowFlags_None);
	if (!hidden_detail)
	{
		std::shared_ptr<Actor> last_selected = selection_actors.empty() ? nullptr : *selection_actors.rbegin();
		if (last_selected != nullptr)
		{
			last_selected->OnGUI();
		}
	}
	ImGui::End();
}

//------------------------------
// �A�N�^�[�擾����
//------------------------------
std::shared_ptr<Actor> ActorManager::GetActor(const char* name)
{
	for (std::shared_ptr<Actor> actor : update_actors)
	{
		if (strcmp(name, actor->GetName()) == 0)
		{
			return actor;
		}
	}
	return nullptr;
}
