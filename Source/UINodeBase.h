#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>

class Texture;
class UIActionBase;
class UI;
class UIData;

//*********************************
// 
// UI�m�[�h�x�[�X�N���X
// 
//*********************************
class UINodeBase
{
public:
	// �R���X�g���N�^
	UINodeBase(std::string name, UINodeBase* parent, UINodeBase* sibling, UIData* ui_data, UIActionBase* action, int hierarchy_no) :
		name(name), parent(parent), sibling(sibling), ui_data(ui_data), action(action), hierarchy_no(hierarchy_no) {}
	
	// ���s����
	void Run(float elapsed_time);

	//�`�揈��
	void Render(ID3D11DeviceContext* context);

	void Rebder(ID3D11DeviceContext* context, Texture* texture, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size, const float& angle);

	// UI�f�[�^����
	UINodeBase* SearchNode(std::string name);

	// �eUI�ݒ�
	UINodeBase* SetParent(UINodeBase* parent) { return this->parent = parent; }

	// �eUI�擾
	UINodeBase* GetParent() const { return this->parent; }

	// �q�m�[�h�ǉ�
	void AddChild(UINodeBase* child) { children.push_back(child); }

	// �q�m�[�h�擾
	const std::vector<UINodeBase*>& GetChildren() { return children; }

	// �q�m�[�h�擾
	UINodeBase* GetChild(int index);

	// �q�m�[�h�擾(����)
	UINodeBase* GetLastChild();

	// �q�m�[�h�擾(�擪)
	UINodeBase* GetTopChild();

	// �Z��m�[�h�ݒ�
	void SetSibling(UINodeBase* sibling) { this->sibling = sibling; }

	// �Z��m�[�h�擾
	UINodeBase* SetSibling() { return sibling; }
	
	// �K�w�ԍ��擾
	int GetHiererchyNo() const { return hierarchy_no; }
private:
	std::string					name;			// ���O�擾
	UIActionBase*				action;			// ���s�N���X
	UINodeBase*					parent;			// �e�m�[�h
	UIData*						ui_data;
	std::vector<UINodeBase*>	children;		// �q�m�[�h
	UINodeBase*					sibling;		// �Z��m�[�h
	int							hierarchy_no;	// �K�w�ԍ�
};
