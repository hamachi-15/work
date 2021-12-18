#include <imgui.h>
#include "Misc.h"
#include "Light.h"
#include "Graphics.h"
#include "ActorManager.h"

#include "PhongVarianceShadowMap.h"
#include "VarianceShadowMap.h"
#include "CascadeShadowMapShader.h"
#include "GaussianBlurShader.h"
#include "PhongShader.h"
#include "GaussianXBlur.h"
#include "GaussianYBlur.h"

#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"

//------------------------------
// �R���X�g���N�^
//------------------------------
ActorManager::ActorManager()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	shadowmap = std::make_unique<VarianceShadowMap>(device);
	bulr = std::make_unique<GaussianBlur>(device);
	//shader = std::make_unique<CascadeShadowMap>(device);
	phong = std::make_unique<Phong>(device);

	// �V���h�E�e�N�X�`���쐬
	for (int i = 0; i < 3; ++i)
	{
		shadow_texture[i] = std::make_unique<Texture>();
		shadow_texture[i]->Create(static_cast<u_int>(shadow_size[i].x), static_cast<u_int>(shadow_size[i].y), DXGI_FORMAT_R32_FLOAT);
		depth_texture[i] = std::make_unique<Texture>();
		depth_texture[i]->CreateDepthStencil(static_cast<u_int>(shadow_size[i].x), static_cast<u_int>(shadow_size[i].y));
	}

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
	Camera& camera = Camera::Instance();

	// �J�����̃p�����[�^�擾
	DirectX::XMVECTOR view_front, view_right, view_up, view_pos;
	{
		view_front = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetFront()));
		view_right = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetRight()));
		view_up = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.GetUp()));
		view_pos = DirectX::XMLoadFloat3(&camera.GetEye());
	}

	// ���C�g�r���[�v���W�F�N�V�����s����Z�o
	DirectX::XMMATRIX	view_matrix, projection_matrix, light_view_projection;
	
	// ���C�g�ʒu����̃r���[�ݒ�
	DirectX::XMFLOAT3 t = { 0, 0, 0 };
	DirectX::XMFLOAT3 pos, target, up;
	pos.x = t.x - Light::LightDir.x * 100.0f;
	pos.y = t.y - Light::LightDir.y * 100.0f;
	pos.z = t.z - Light::LightDir.z * 100.0f;
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };
	DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR vtarget = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR vup = DirectX::XMLoadFloat3(&up);

	view_matrix = DirectX::XMMatrixLookAtLH(eye, vtarget, vup);

	projection_matrix = DirectX::XMMatrixOrthographicLH(10000.0f, 10000.0f, 0.01f, 2000.0f);
	light_view_projection = view_matrix * projection_matrix;
	

	//	�V���h�E�}�b�v�̕����G���A��`
	float	split_area_table[] =
	{
		camera.GetNear(),	//	�J�����̃j�A�N���b�v
		shadow_area[0],
		shadow_area[1],
		shadow_area[2]
	};
	for (int i = 0; i < 2; ++i)
	{
		float near_depth = split_area_table[i + 0];
		float far_depth = split_area_table[i + 1];

		// �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̐ݒ�
		ID3D11RenderTargetView* render_target_view[1] = { shadow_texture[i]->GetRenderTargetView() };
		ID3D11DepthStencilView* depth_stencil_view = depth_texture[i]->GetDepthStencilView();
		graphics.SetRenderTargetView(render_target_view, depth_stencil_view);

		// ��ʃN���A
		graphics.ScreenClear(render_target_view, depth_stencil_view);

		// �r���[�|�[�g�̐ݒ�
		graphics.SetViewport(shadow_size[i].x, shadow_size[i].y);

		// �G���A������8���_���Z�o����
		DirectX::XMVECTOR	vertex[8];
		{
			//	�G���A�̋ߕ��ʂ̒��S����̏�ʂ܂ł̋��������߂�
			float	nearY = tanf(camera.GetFov() * 0.5f) * near_depth;
			//	�G���A�̋ߕ��ʂ̒��S����̉E�ʂ܂ł̋��������߂�
			float	nearX = nearY * camera.GetAspect();
			//	�G���A�̉����ʂ̒��S����̏�ʂ܂ł̋��������߂�
			float	farY = tanf(camera.GetFov() * 0.5f) * far_depth;
			//	�G���A�̉����ʂ̒��S����̉E�ʂ܂ł̋��������߂�
			float	farX = farY * camera.GetAspect();

			//	�G���A�̋ߕ��ʂ̒��S���W�����߂�
			DirectX::XMVECTOR	near_position = DirectX::XMVectorAdd(view_pos, DirectX::XMVectorScale(view_front, near_depth));
			//	�G���A�̉����ʂ̒��S���W�����߂�
			DirectX::XMVECTOR	far_position = DirectX::XMVectorAdd(view_pos, DirectX::XMVectorScale(view_front, far_depth));

			//	8���_�����߂�
			{
				// �ߕ��ʂ̉E��
				vertex[0] = DirectX::XMVectorAdd(near_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, nearY), DirectX::XMVectorScale(view_right, nearX)));
				// �ߕ��ʂ̍���
				vertex[1] = DirectX::XMVectorAdd(near_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, nearY), DirectX::XMVectorScale(view_right, -nearX)));
				// �ߕ��ʂ̉E��
				vertex[2] = DirectX::XMVectorAdd(near_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, -nearY), DirectX::XMVectorScale(view_right, nearX)));
				// �ߕ��ʂ̍���
				vertex[3] = DirectX::XMVectorAdd(near_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, -nearY), DirectX::XMVectorScale(view_right, -nearX)));
				// �����ʂ̉E��
				vertex[4] = DirectX::XMVectorAdd(far_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, farY), DirectX::XMVectorScale(view_right, farX)));
				// �����ʂ̍���
				vertex[5] = DirectX::XMVectorAdd(far_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, farY), DirectX::XMVectorScale(view_right, -farX)));
				// �����ʂ̉E��
				vertex[6] = DirectX::XMVectorAdd(far_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, -farY), DirectX::XMVectorScale(view_right, farX)));
				// �����ʂ̍���
				vertex[7] = DirectX::XMVectorAdd(far_position, DirectX::XMVectorAdd(DirectX::XMVectorScale(view_up, -farY), DirectX::XMVectorScale(view_right, -farX)));
			}
		}
		//	8���_�����C�g�r���[�v���W�F�N�V������Ԃɕϊ����āA�ő�l�A�ŏ��l�����߂�
		DirectX::XMFLOAT3	vertex_min(FLT_MAX, FLT_MAX, FLT_MAX), vertex_max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (auto& it : vertex)
		{
			DirectX::XMFLOAT3	p;
			DirectX::XMStoreFloat3(&p, DirectX::XMVector3TransformCoord(it, light_view_projection));

			vertex_min.x = min(p.x, vertex_min.x);
			vertex_min.y = min(p.y, vertex_min.y);
			vertex_max.x = max(p.x, vertex_max.x);
			vertex_max.y = max(p.y, vertex_max.y);
		}

		// �N���b�v�s������߂�
		DirectX::XMMATRIX	clop_matrix = DirectX::XMMatrixIdentity();
		{
			float	xscale = 2.0f / (vertex_max.x - vertex_min.x);
			float	yscale = 2.0f / (vertex_max.y - vertex_min.y);
			float	xoffset = -0.5f * (vertex_max.x + vertex_min.x) * xscale;
			float	yoffset = -0.5f * (vertex_max.y + vertex_min.y) * yscale;
			DirectX::XMFLOAT4X4	float_clop_matrix;
			DirectX::XMStoreFloat4x4(&float_clop_matrix, clop_matrix);
			float_clop_matrix._11 = xscale;
			float_clop_matrix._22 = yscale;
			float_clop_matrix._41 = xoffset;
			float_clop_matrix._42 = yoffset;
			clop_matrix = DirectX::XMLoadFloat4x4(&float_clop_matrix);
		}
		//���C�g�r���[�v���W�F�N�V�����s����v�Z
		DirectX::XMStoreFloat4x4(&render_context.light_view_projection[i], light_view_projection * clop_matrix);
		render_context.slight_view_projection = render_context.light_view_projection[i];

		shadowmap->Begin(context, render_context);
		for (std::shared_ptr<Actor>& actor : update_actors)
		{
			//if (strcmp(actor->GetName(), "Stage") == 0) continue;
			// ���f��������Ε`��
			Model* model = actor->GetModel();
			if (model != nullptr)
			{
				shadowmap->Draw(context, model);
			}
		}
		shadowmap->End(context);

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

	for (std::shared_ptr<Actor>& actor : update_actors)
	{
		// ���݃Z�b�g����Ă���V�F�[�_�[�Ƃ��ꂩ��̕`��Ɏg���V�F�[�_�[��������
		if(strcmp(shader_name.c_str(),actor->GetShader()->GetShaderName()) != 0)
		{	
			// �V�F�[�_�[�̏I������
			if(shader != nullptr)
				shader->End(context);
			// ���݂̃V�F�[�_�[�����ւ���
			shader = actor->GetShader();
			shader_name = shader->GetShaderName();
			shader->Begin(context, render_context);
		}
		// �V�F�[�_�[�Ƀ|�C���^�������Ă��Ȃ���΃A�T�[�g
		_ASSERT_EXPR_A(shader, "shader is nullptr");

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
// �P�x���Z�o���郂�f����`�悷��
//------------------------------
void ActorManager::BrightRender(RenderContext& render_context)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	phong->Begin(context, render_context, DirectX::XMFLOAT4{2, 2, 2, 0.7f});
	for (std::shared_ptr<Actor>& actor : update_actors)
	{
		if (strcmp(actor->GetName(), "Stage") == 0) continue;
		// ���f��������Ε`��
		Model* model = actor->GetModel();
		if (model != nullptr)
		{
			phong->Draw(context, model);
		}
	}
	phong->End(context);
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
