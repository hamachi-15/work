#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>

class ModelResource;
// モデル
class Model
{
public:
	Model(const char* filename, const char* ignore_root_motion_node_name);
	~Model(){}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4 local_transform;
		DirectX::XMFLOAT4X4 world_transform;
		std::vector<Node*>	children;
	};
	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	
	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	
	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }
	
	// ノード検索
	Node* FindNode(const char* name);
	
	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time, bool hitstop_flag = false);
	
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	
	// 現在のアニメーション再生時間取得
	float GetCurrentAnimationSeconds() const { return current_animation_seconds; }
	
	// アニメーション再生中か
	bool IsPlayAnimation() const;

	// アニメーション再生の停止処理
	void AnimationStop() { animation_stop_flag = true; }
	
	// アニメーションの再再生処理
	void AnimationReplay() { animation_stop_flag = false; }

	const DirectX::XMFLOAT3 GetAnimationPosition() const { return animation_position; }
	void SetAnimationPosition(DirectX::XMFLOAT3 position) { animation_position = position; }
	
	// ヒットストップ処理
	float HitStop(float t, float t0, float t1, float k);
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
	int								current_animation_index = -1;
	float							current_animation_seconds = 0.0f;
	bool							animation_loop_flag = false;
	bool							animation_end_flag = false;
	bool							animation_stop_flag = false;
	float							animation_blend_time = 0.0f;
	float							animation_blend_seconds = 0.0f;
	DirectX::XMFLOAT3				animation_position = {};
};