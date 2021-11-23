#include "Model.h"
#include "Graphics.h"
#include "ModelResource.h"
#include "ResourceManager.h"
#include "ActorManager.h"
//-------------------------------------
// �R���X�g���N�^
//-------------------------------------
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& res_nodes = resource->GetNodes();

	nodes.resize(res_nodes.size());
	for (size_t node_index = 0; node_index < nodes.size(); ++node_index)
	{
		auto& src = res_nodes.at(node_index);
		auto& dst = nodes.at(node_index);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

//-------------------------------------
// �s��v�Z
//-------------------------------------
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX _transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX s = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX local_transform = s * r * t;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX parent_transform;
		if (node.parent != nullptr)
		{
			parent_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
		}
		else
		{
			parent_transform = _transform;
		}
		DirectX::XMMATRIX world_transform = local_transform * parent_transform;

		// �v�Z���ʂ�ۑ�
		DirectX::XMStoreFloat4x4(&node.world_transform, world_transform);
		DirectX::XMStoreFloat4x4(&node.local_transform, local_transform);
	}
}

//-------------------------------------
// �m�[�h����
//-------------------------------------
Model::Node* Model::FindNode(const char* name)
{
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r
	for (Node& node : nodes)
	{
		if (strcmp(node.name, name) == 0)
		{
			return &node;
		}
	}
	return nullptr;
}

//-------------------------------------
// �A�j���[�V�����X�V����
//-------------------------------------
void Model::UpdateAnimation(float elapsed_time, const char* node_name, bool hitstop_flag)
{
	// �Đ����Ȃ�X�V���Ȃ�
	if (!IsPlayAnimation() || animation_stop_flag) return;

	// �u�����h���̌v�Z
	float blend_rate = 10.0f;
	if (animation_blend_time < animation_blend_seconds)
	{
		animation_blend_time += elapsed_time;
		if (animation_blend_time >= animation_blend_seconds)
		{
			animation_blend_time = animation_blend_seconds;
		}
		blend_rate = animation_blend_time / animation_blend_seconds;
		blend_rate *= blend_rate;
	}
	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int key_count = static_cast<int>(keyframes.size());
	for (int keyindex = 0; keyindex < key_count - 1; ++keyindex)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩�m�F����
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyindex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyindex + 1);
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ����ԗ����Z�o����
			float rate = (current_animation_seconds - keyframe0.seconds /
				keyframe1.seconds - keyframe0.seconds);

			int node_count = static_cast<int>(nodes.size());
			for (int node_index = 0; node_index < node_count; ++node_index)
			{
				// 2�̃L�[�t���[���Ԃ̕�Ԍv�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(node_index);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(node_index);

				Node& node = nodes[node_index];
				if (blend_rate < 1.0f)
				{

					DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR r0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR r1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR t0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR s = DirectX::XMVectorLerp(s0, s1, blend_rate);
					DirectX::XMVECTOR r = DirectX::XMQuaternionSlerp(r0, r1, blend_rate);
					DirectX::XMVECTOR t = DirectX::XMVectorLerp(t0, t1, blend_rate);

					DirectX::XMStoreFloat3(&node.scale, s);
					DirectX::XMStoreFloat4(&node.rotate, r);
					DirectX::XMFLOAT3 p = { 0, 0, 0 };
					DirectX::XMStoreFloat3(&p, t);
					if (strcmp(node.name, node_name) != 0) {
						DirectX::XMStoreFloat3(&node.translate, t);
					}
				}
				// �ʏ�̌v�Z
				else
				{

					DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR r0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR r1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR t0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR s = DirectX::XMVectorLerp(s0, s1, rate);
					DirectX::XMVECTOR r = DirectX::XMQuaternionSlerp(r0, r1, rate);
					DirectX::XMVECTOR t = DirectX::XMVectorLerp(t0, t1, rate);
					
					DirectX::XMFLOAT3 p; DirectX::XMStoreFloat3(&p, t);
					DirectX::XMStoreFloat3(&node.scale, s);
					DirectX::XMStoreFloat4(&node.rotate, r);
					if (strcmp(node.name, node_name) != 0) {
						DirectX::XMStoreFloat3(&node.translate, t);
					}
				}
			}
			break;
		}
	}
	// �ŏI�t���[������
	if (animation_end_flag)
	{
		animation_end_flag = true;
		current_animation_index = -1;
	}
	// �o�ߎ���
	//current_animation_seconds += elapsed_time;
	// �q�b�g�X�g�b�v���Ԓ��Ȃ玞�Ԃ̗������������
	//if (hitstop_flag)
	//{
	//	current_animation_seconds += (elapsed_time * 0.1f);
	//}
	//else
	{
		current_animation_seconds += elapsed_time;
	}
	// �Đ����Ԃ��I�[���Ԃ𒴂�����
		if (current_animation_seconds >= animation.secondsLength)
	{
		if (animation_loop_flag)
		{
			// �Đ����Ԃ������߂�
			current_animation_seconds -= animation.secondsLength;
		}
		else
		{
			current_animation_seconds = animation.secondsLength;
			animation_end_flag = true;
		}
	}
}

//-------------------------------------
// �A�j���[�V�����Đ�
//-------------------------------------
void Model::PlayAnimation(int index, bool loop, float blend_seconds)
{
	// �A�j���[�V�����̏����ݒ�
	current_animation_index = index;
	current_animation_seconds = 0.0f;
	animation_loop_flag = loop;
	animation_end_flag = false;
	animation_blend_time = 0.0f;
	animation_blend_seconds = blend_seconds;
}

//-------------------------------------
// �A�j���[�V�����Đ�����
//-------------------------------------
bool Model::IsPlayAnimation() const
{
	if (current_animation_index < 0) return false;
	if (current_animation_index >= resource->GetAnimations().size()) return false;
	return true;
}

//-------------------------------------
// �q�b�g�X�g�b�v
//-------------------------------------
float Model::HitStop(float t, float t0, float t1, float k)
{
	if (t < t0) { return t; }//float elapsed_time, float start_time, float end_time, float coefficient
	if (t0 <= t && t <= t1) { return k * (t - t0) + t0; }
	return  (t - t1) + (k * (t1 - t0) + t0);
}