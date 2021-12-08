#include "ResourceManager.h"
#include "Graphics.h"

//-------------------------------------
// モデルリソース読み込み
//-------------------------------------
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	// モデル検索
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		if (!it->second.expired())
		{
			return it->second.lock();
		}
	}
	// 新規モデルリソース作成と読み込み
	ID3D11Device* device = Graphics::Instance().GetDevice();
	std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
	model->Load(device, filename);

	// マップに登録
	models[filename] = model;

	return model;
}

//-------------------------------------
// テクスチャ読み込み
//-------------------------------------
std::shared_ptr<Texture> ResourceManager::LoadTexture(const char* filename)
{
	TextureMap::iterator it = textures.find(filename);
	if (it != textures.end())
	{
		if (!it->second.expired())
		{
			return it->second.lock();
		}
	}
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Load(filename);

	// マップに登録
	textures[filename] = texture;

	return texture;
}
