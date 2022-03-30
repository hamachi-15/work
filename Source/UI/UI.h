#pragma once
#include <string>
#include <d3d11.h>
#include <memory>
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
	UI();

	// �f�X�g���N�^
	virtual ~UI();

	// ���O�擾
	virtual std::string GetName() const = 0;

	// �J�n����
	virtual void Start() = 0;

	// �X�V����
	void Update(float elapsed_time);
	
	// �X�V����
	void Render(ID3D11DeviceContext* context);

public:
	std::unique_ptr<UINodeTree> ui_node_tree = nullptr;
};