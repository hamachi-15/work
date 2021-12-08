#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include "DirectXMath.h"
#include <d3d11.h>

class Texture;
class UI;

struct UIParameter
{
	const char*					filename;
	std::string					name;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	float						angle;
	std::shared_ptr<UI>			parent = nullptr;
};


class UI
{
public:
	// �R���X�g���N�^
	UI(const UIParameter& parameter);

	// �f�X�g���N�^
	virtual ~UI();
	
	//
	void SetParameter(const UIParameter& parameter);

	// ���O�ݒ�
	void SetName(std::string name) { this->name = name; }

	// ���O�擾
	const char* GetName() const { return name.c_str(); }

	// �X�V����
	virtual void Update(float elapsed_time) = 0;

	// �`����W�ݒ�
	void SetPosition(DirectX::XMFLOAT2 position) { this->position = position; }

	// �`����W�擾
	const DirectX::XMFLOAT2& GetPosition() const { return position; }

	// �`��T�C�Y�ݒ�
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	// �`��T�C�Y�擾
	const DirectX::XMFLOAT2& GetSize() const { return size; }

	// �p�x�ݒ�
	void SetAngle(float angle) { this->angle = angle; }

	// �p�x�擾
	const float& GetAngle() const { return angle; }

	// �e�N�X�`���ݒ�
	void SetTexture(const char* filename);

	// �e�N�X�`���擾
	Texture* GetTexture() const { return texture.get(); }

	// �eUI�ݒ�
	std::shared_ptr<UI> SetParent(std::shared_ptr<UI> parent) { return this->parent = parent; }

	// �eUI�擾
	std::shared_ptr<UI> GetParent() const { return this->parent; }

	// ���[���h���W����X�N���[�����W�ɕϊ�����
	DirectX::XMFLOAT2 ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
		const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:
	std::string					name;
	std::shared_ptr<Texture>	texture;
	DirectX::XMFLOAT2			position;
	DirectX::XMFLOAT2			size;
	float						angle;
	std::shared_ptr<UI>			parent = nullptr; // �e��UI
};

class UIManager
{
private:
	// �R���X�g���N�^
	UIManager();
	// �f�X�g���N�^
	~UIManager();
public:
	// �C���X�^���X�擾
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Draw(ID3D11DeviceContext* context);

	// UI�o�^
	void RegisterUI(std::shared_ptr<UI> ui);

	// UI�폜
	void UnRegisterUI(std::shared_ptr<UI> ui);
private:
	std::vector<std::shared_ptr<UI>> ui_list;
	std::vector<std::shared_ptr<UI>> remove;
};