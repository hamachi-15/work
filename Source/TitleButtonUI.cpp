#include "TitleButtonUI.h"
#include "UINodeTree.h"
#include "UIData.h"
#include "Graphics.h"
#include "TitleButtonUIAction.h"
#include "TitleUIJudgment.h"

//********************************
// 
// �^�C�g���̃{�^��UI
// 
//********************************
//--------------------------------
// �f�X�g���N�^
//--------------------------------
TitleUI::~TitleUI()
{
	delete select_start_button_data;
	delete start_data;
	delete start_button_data;

	delete operate_button_data;
	delete select_operate_button_data;
	delete operate_data;

	delete title_logo;
}

//--------------------------------
// �J�n����
//--------------------------------
void TitleUI::Start()
{
	Graphics& graphics = Graphics::Instance();
	DirectX::XMFLOAT2 screen_size = { graphics.GetScreenWidth(), graphics.GetScreenHeight() };
	DirectX::XMFLOAT2 start_position = { screen_size.x * 0.15f, screen_size.y - (screen_size.y * 0.25f) };
	
	UIParameter parameter;

	// �^�C�g�����S�̃f�[�^
	parameter.filename = "Data/Sprite/TitleLogo.png";
	parameter.name = "TitleLogo";
	parameter.position = DirectX::XMFLOAT2(300, 100);
	parameter.scale = { 1.0f, 1.0f };
	title_logo = new UIData(parameter);

	// ��I�����̃X�^�[�g�{�^���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/03 button_rectangle/btn_rectangle_bl.png";
	parameter.name = "StartButton";
	parameter.position = start_position;
	parameter.scale = { 0.8f, 0.8f };
	start_button_data = new UIData(parameter);

	// �I�����̃X�^�[�g�{�^���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/03 button_rectangle/btn_rectangle_bl_click.png";
	parameter.name = "SelectStartButton";
	parameter.position = start_position;
	select_start_button_data = new UIData(parameter);


	DirectX::XMFLOAT2 operate_position = { screen_size.x * 0.55f, screen_size.y - (screen_size.y * 0.25f) };
	// ��I�����̃X�^�[�g�{�^���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/03 button_rectangle/btn_rectangle_gr.png";
	parameter.name = "StartButton";
	parameter.position = operate_position;
	operate_button_data = new UIData(parameter);

	// �I�����̃X�^�[�g�{�^���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/03 button_rectangle/btn_rectangle_gr_click.png";
	parameter.name = "SelectStartButton";
	parameter.position = operate_position;
	select_operate_button_data = new UIData(parameter);

	// Start�̕����̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/Start.png";
	parameter.name = "Start";
	parameter.position = { start_position.x + 60, start_position.y + 20 };
	parameter.scale = { 1.0f, 1.0f };
	start_data = new UIData(parameter);
	
	// Operate�̕����̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/Dummy.png";
	parameter.name = "Start";
	parameter.position = { operate_position.x + 50, operate_position.y + 20 };
	operate_data = new UIData(parameter);

	ui_node_tree = new UINodeTree();
	// UI�ݒ�
	ui_node_tree->AddNode("", "root", NULL, NULL, NULL);
	ui_node_tree->AddNode("root", "Button", NULL, NULL, new TitleButtonUIAction(nullptr));
	ui_node_tree->AddNode("root", "Title", NULL, NULL, NULL);
	ui_node_tree->AddNode("Button", "StartButton", NULL, NULL, NULL);
	ui_node_tree->AddNode("Button", "OperateButton", NULL, NULL, NULL);
	ui_node_tree->AddNode("Title", "TitleLogo", title_logo, NULL, NULL);
	ui_node_tree->AddNode("StartButton", "UnselectStartButton", start_button_data, NULL, NULL);
	ui_node_tree->AddNode("StartButton", "SelectStartButton", select_start_button_data, new StartButtonSelectDrawJudgment(select_start_button_data), NULL);
	ui_node_tree->AddNode("StartButton", "Start", start_data, NULL, NULL);
	ui_node_tree->AddNode("OperateButton", "UnselectStartButton", operate_button_data, NULL, NULL);
	ui_node_tree->AddNode("OperateButton", "SelectStartButton", select_operate_button_data, new OperateButtonSelectDrawJudgment(select_operate_button_data), NULL);
	ui_node_tree->AddNode("OperateButton", "Operate", operate_data, NULL, NULL);
}
