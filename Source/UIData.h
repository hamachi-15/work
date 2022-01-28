#pragma once
#include <memory>
#include <DirectXMath.h>
#include "ResourceManager.h"

class Charactor;

struct UIParameter
{
	std::shared_ptr<Charactor>	charactor = nullptr;
	const char*					filename = "";
	std::string					name = "";
	DirectX::XMFLOAT2			position = {};
	DirectX::XMFLOAT2			scale = {};
	float						angle = 0.0f;
};
//***********************************
// 
// UI�f�[�^�N���X
// 
//***********************************
class UIData
{
public:
	// �R���X�g���N�^
	UIData(const UIParameter& parameter) :
		name(parameter.name), position(parameter.position), angle(parameter.angle),charactor(parameter.charactor),
		texture(ResourceManager::Instance().LoadTexture(parameter.filename)), scale(parameter.scale), 
		size({ static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight())}) {}

	~UIData(){}

	// �L�����N�^�R���|�[�l���g�ݒ�
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }

	// �L�����N�^�R���|�[�l���g�擾
	std::shared_ptr<Charactor> GetCharactor() const { return charactor; }

	// �`����W�ݒ�
	void SetPosition(DirectX::XMFLOAT2 position) { this->position = position; }

	// �`����W�擾
	const DirectX::XMFLOAT2& GetPosition() const { return position; }

	// �`��T�C�Y�ݒ�
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	// �`��T�C�Y�擾
	const DirectX::XMFLOAT2& GetSize() const { return size; }

	// �X�P�[���ݒ�
	void SetScale(DirectX::XMFLOAT2 scale) { this->scale = scale; }
	
	// �X�P�[���擾
	const DirectX::XMFLOAT2& GetScale() const { return scale; }

	// �p�x�ݒ�
	void SetAngle(float angle) { this->angle = angle; }

	// �p�x�擾
	const float& GetAngle() const { return angle; }

	// �e�N�X�`���ݒ�
	void SetTexture(const char* filename);

	// �e�N�X�`���擾
	Texture* GetTexture() const { return texture.get(); }
private:
	std::string					name;
	std::shared_ptr<Texture>	texture;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	DirectX::XMFLOAT2			scale;
	float						angle;
	std::shared_ptr<Charactor>	charactor;
};