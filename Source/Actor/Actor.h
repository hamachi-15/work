#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Shader.h"
#include "Model.h"
#include "ShaderManager.h"

class Component;
//class Model;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	// コンストラクタ・デストラクタ
	Actor() {}
	virtual ~Actor() {}

	// 開始処理
	virtual void Start();

	// 更新処理
	virtual void Update(float elapsed_time);

	// 行列更新
	virtual void UpdateTransform();

	// IMGUI表示
	virtual void OnGUI();

	// モデルのセットアップ
	void SetUpModel(const char* filename, const char* ignore_root_motion_node_name);

	// モデルの取得
	Model* GetModel() const { return model.get(); }

	// シェーダーの設定
	void SetShaderType(ShaderManager::ShaderType shader_type) { this->shader_type = shader_type; }

	// シェーダーの取得
	ShaderManager::ShaderType GetShaderType() { return shader_type; }

	// 名前の設定
	void SetName(const char* name) { this->name = name; }

	// 名前の取得
	const char* GetName() const { return name.c_str(); }
	std::string GetNameOfString() const { return name; }

	// 位置の設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 位置の取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 回転の設定
	void SetRotation(const DirectX::XMFLOAT4& rotation) { this->rotation = rotation; }

	// 角度の設定
	void SetAngle(const DirectX::XMFLOAT3& angle);

	// 回転の取得
	const DirectX::XMFLOAT4& GetRotation() const { return rotation; }

	// 角度の取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 角度の加算
	void AddAngle(const DirectX::XMFLOAT3& add_angle);
	
	// 角度の減算
	void SubtractAngle(const DirectX::XMFLOAT3& add_angle);

	// スケールの設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// スケールの取得
	const DirectX::XMFLOAT3 GetScale() const { return scale; }

	// カリングを行うかのフラグ
	void SetCullingFlag(bool culling_flag) { this->culling_flag = culling_flag; }

	// カリングを行うかのフラグ取得
	const bool GetCullingFlag() const { return culling_flag; }

	// 行列の取得
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// アニメーションフラグ設定
	void SetAnimationFlag(bool animation_flag) { this->animation_flag = animation_flag; }
	
	// アニメーションフラグ取得
	bool SetAnimationFlag() { return animation_flag; }


	// コンポーネント設定
	template<class T, class...Args>
	std::shared_ptr<T> AddComponent(Args... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);
		component->SetActor(shared_from_this());
		components.emplace_back(component);
		return component;
	}

	// コンポーネント取得
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for (std::shared_ptr<Component>& component : components)
		{
			std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(component);
			if (p == nullptr) continue;
			return p;
		}
		return nullptr;
	}
private:
	std::string				name;
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT4		rotation = { 0, 0, 0, 1 }; // クォータニオン用
	DirectX::XMFLOAT3		angle = { 0, 0, 0 }; // オイラー角用
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4		transform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	bool					culling_flag;

	std::unique_ptr<Model>	model;
	bool					animation_flag = true; // アニメーション更新を行うかのフラグ
	ShaderManager::ShaderType		shader_type;
	std::vector<std::shared_ptr<Component>>	components;
};