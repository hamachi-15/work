#pragma once

#include "Shader.h"

#include <vector>
class ShaderManager
{
public:
	enum class ShaderType
	{
		Sprite,
		Primitive,
		SkyBox,
		Bulr,
		Bloom,
		Phong,
		Lambert,
		CreateShadowMap,
		CascadeShadowMap,
		VarianceShadowMap
	};
private:
	// �R���X�g���N�^
	ShaderManager() {}
	// �f�X�g���N�^
	~ShaderManager() {}
public:
	// �C���X�^���X�擾
	static ShaderManager& Instance()
	{
		static ShaderManager instance;
		return instance;
	}

	// �V�F�[�_�[�擾
	std::shared_ptr<Shader> GetShader(ShaderType type) { return shaders.at(static_cast<int>(type)); }
	
	// �V�F�[�_�[�ǉ�
	template<class T, class...Args>
	void RegisterShader(Args... args)
	{
		std::shared_ptr<T> shader = std::make_shared<T>(args...);
		shaders.emplace_back(shader);
	}
private:
	std::vector<std::shared_ptr<Shader>> shaders;

};