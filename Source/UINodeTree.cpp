#include "UINodeTree.h"
#include "UINodeBase.h"
//--------------------------
// �f�X�g���N�^
//--------------------------
UINodeTree::~UINodeTree()
{
	NodeAllClear(root);
}

//--------------------------
// UI�m�[�h�o�^
//--------------------------
void UINodeTree::AddNode(std::string search_name, std::string entry_name, UIData* ui_data, UIActionBase* ui_action)
{
	// �T�[�`���閼�O���Ȃ����
	if (search_name == "")
	{
		// ���̂܂܃m�[�h��ǉ�
		if (root == NULL)
		{
			root = new UINodeBase(entry_name, NULL, NULL, ui_data, ui_action, 1);
		}
	}
	else
	{
		// �m�[�h������
		UINodeBase* search_node = root->SearchNode(search_name);
		// �����������ʌ������
		if (search_node != NULL)
		{
			// �Z��ɂȂ�m�[�h���擾��
			UINodeBase* sibling = search_node->GetLastChild();
			// �ǉ�����m�[�h���쐬��
			UINodeBase* add_node = new UINodeBase(entry_name, search_node, sibling, ui_data, ui_action, search_node->GetHiererchyNo() + 1);
			// �����Ƀq�b�g�����m�[�h�̎q�m�[�h�ɒǉ�
			search_node->AddChild(add_node);
		}
	}

}

//--------------------------
// ���s
//--------------------------
void UINodeTree::Run(float elapsed_time)
{
	// ���s����
	root->Run(elapsed_time);
}

// �`�揈��
void UINodeTree::Render(ID3D11DeviceContext* context)
{
	root->Render(context);
}

//--------------------------
// �m�[�h�S�폜
//--------------------------
void UINodeTree::NodeAllClear(UINodeBase* del_node)
{
	size_t count = del_node->GetChildren().size();
	if (count > 0)
	{
		for (UINodeBase* node : del_node->GetChildren())
		{
			NodeAllClear(node);
		}
		delete del_node;
	}
	else
	{
		delete del_node;
	}
}
