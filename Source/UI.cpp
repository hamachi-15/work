#include "UI.h"
#include "UIActionBase.h"
#include "UINodeTree.h"

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
UI::~UI()
{
	delete ui_node_tree;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UI::Update(float elapsed_time)
{
	// 更新処理
	ui_node_tree->Run(elapsed_time);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UI::Render(ID3D11DeviceContext* context)
{
	ui_node_tree->Render(context);
}

