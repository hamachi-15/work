#pragma once
#include <memory>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

class UINodeTree;
class Charactor;

//******************************
// 
// UI�N���X
// 
//******************************
class UI
{
public:
	// �R���X�g���N�^
	UI() {}

	// �f�X�g���N�^
	virtual ~UI();

	// ���O�ݒ�
	void SetName(std::string name) { this->name = name; }

	// ���O�擾
	const char* GetName() const { return name.c_str(); }

	// �J�n����
	virtual void Start() = 0;

	// �X�V����
	void Update(float elapsed_time);
	
	// �X�V����
	void Render(ID3D11DeviceContext* context);

	// ���[���h���W����X�N���[�����W�ɕϊ�����
	DirectX::XMFLOAT2 ScreenPositionOfWorldPosition(ID3D11DeviceContext* context,
		const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:
	std::string name;
public:
	UINodeTree* ui_node_tree = nullptr;
};