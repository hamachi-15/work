#pragma once
#include <memory>
#include <string>
#include <map>
#include "ModelResource.h"
#include "Texture.h"

class ResourceManager
{
private:
	ResourceManager(){}
	~ResourceManager(){}

public:
	// �C���X�^���X�擾
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}
	// ���f�����\�[�X�ǂݍ���
	std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

	// �e�N�X�`���ǂݍ���
	std::shared_ptr<Texture> LoadTexture(const char* filename);

private:
	using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;
	ModelMap models;
	using TextureMap = std::map<std::string, std::weak_ptr<Texture>>;
	TextureMap textures;
};