#pragma once
#include <string>
#include <d3d11.h>

class UIActionBase;
class UIJudgmentBase;
class UINodeBase;
class UIData;

class UINodeTree
{
public:
	// �R���X�g���N�^
	UINodeTree() : root(NULL) {}

	// �f�X�g���N�^
	~UINodeTree();

	// UI�m�[�h�o�^
	void AddNode(std::string entry_name, std::string search_name, UIData* ui_data, UIJudgmentBase* ui_judgment, UIActionBase* ui_action);

	// ���s
	void Run(float elapsed_time);

	// �`�揈��
	void Render(ID3D11DeviceContext* context);

private:
	// �m�[�h�S�폜
	void NodeAllClear(UINodeBase* del_node);
private:
	UINodeBase* root;
};