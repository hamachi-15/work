#include "UI.h"
#include "UIActionBase.h"
#include "UINodeTree.h"

UI::UI()
{
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
UI::~UI()
{
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void UI::Update(float elapsed_time)
{
	// �X�V����
	ui_node_tree->Run(elapsed_time);
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void UI::Render(ID3D11DeviceContext* context)
{
	ui_node_tree->Render(context);
}

