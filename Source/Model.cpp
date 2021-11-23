#include "Model.h"
#include "Graphics.h"
#include "ModelResource.h"
#include "ResourceManager.h"
#include "ActorManager.h"
//-------------------------------------
// コンストラクタ
//-------------------------------------
Model::Model(const char* filename)
{
	// リソース読み込み
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// ノード
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

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

//-------------------------------------
// 行列計算
//-------------------------------------
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX _transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX s = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX t = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX local_transform = s * r * t;

		// ワールド行列算出
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

		// 計算結果を保存
		DirectX::XMStoreFloat4x4(&node.world_transform, world_transform);
		DirectX::XMStoreFloat4x4(&node.local_transform, local_transform);
	}
}

//-------------------------------------
// ノード検索
//-------------------------------------
Model::Node* Model::FindNode(const char* name)
{
	// 全てのノードを総当たりで名前比較
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
// アニメーション更新処理
//-------------------------------------
void Model::UpdateAnimation(float elapsed_time, const char* node_name, bool hitstop_flag)
{
	// 再生中なら更新しない
	if (!IsPlayAnimation() || animation_stop_flag) return;

	// ブレンド率の計算
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
	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(current_animation_index);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int key_count = static_cast<int>(keyframes.size());
	for (int keyindex = 0; keyindex < key_count - 1; ++keyindex)
	{
		// 現在の時間がどのキーフレームの間にいるか確認する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyindex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyindex + 1);
		if (current_animation_seconds >= keyframe0.seconds &&
			current_animation_seconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間から補間率を算出する
			float rate = (current_animation_seconds - keyframe0.seconds /
				keyframe1.seconds - keyframe0.seconds);

			int node_count = static_cast<int>(nodes.size());
			for (int node_index = 0; node_index < node_count; ++node_index)
			{
				// 2つのキーフレーム間の補間計算
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
				// 通常の計算
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
	// 最終フレーム処理
	if (animation_end_flag)
	{
		animation_end_flag = true;
		current_animation_index = -1;
	}
	// 経過時間
	//current_animation_seconds += elapsed_time;
	// ヒットストップ時間中なら時間の流れをゆっくりに
	//if (hitstop_flag)
	//{
	//	current_animation_seconds += (elapsed_time * 0.1f);
	//}
	//else
	{
		current_animation_seconds += elapsed_time;
	}
	// 再生時間が終端時間を超えたら
		if (current_animation_seconds >= animation.secondsLength)
	{
		if (animation_loop_flag)
		{
			// 再生時間を巻き戻す
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
// アニメーション再生
//-------------------------------------
void Model::PlayAnimation(int index, bool loop, float blend_seconds)
{
	// アニメーションの初期設定
	current_animation_index = index;
	current_animation_seconds = 0.0f;
	animation_loop_flag = loop;
	animation_end_flag = false;
	animation_blend_time = 0.0f;
	animation_blend_seconds = blend_seconds;
}

//-------------------------------------
// アニメーション再生中か
//-------------------------------------
bool Model::IsPlayAnimation() const
{
	if (current_animation_index < 0) return false;
	if (current_animation_index >= resource->GetAnimations().size()) return false;
	return true;
}

//-------------------------------------
// ヒットストップ
//-------------------------------------
float Model::HitStop(float t, float t0, float t1, float k)
{
	if (t < t0) { return t; }//float elapsed_time, float start_time, float end_time, float coefficient
	if (t0 <= t && t <= t1) { return k * (t - t0) + t0; }
	return  (t - t1) + (k * (t1 - t0) + t0);
}