#include "ResourceManager.h"
#include "Graphics.h"

//-------------------------------------
// ���f�����\�[�X�ǂݍ���
//-------------------------------------
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	// ���f������
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		if (!it->second.expired())
		{
			return it->second.lock();
		}
	}
	// �V�K���f�����\�[�X�쐬�Ɠǂݍ���
	ID3D11Device* device = Graphics::Instance().GetDevice();
	std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
	model->Load(device, filename);

	// �}�b�v�ɓo�^
	models[filename] = model;

	return model;
}