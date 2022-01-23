#include "PlayerUIHealth.h"
#include "UINodeTree.h"
#include "PlayerUIAction.h"
#include "UIData.h"
#include "ActorManager.h"
#include "Charactor.h"

// �f�X�g���N�^
PlayerHealthGage::~PlayerHealthGage()
{
}

void PlayerHealthGage::Start()
{
	// ���O�ݒ�
	SetName("PlayerHealthGage");

	std::shared_ptr<Charactor> charactor = ActorManager::Instance().GetActor("Player")->GetComponent<Charactor>();
	UIParameter parameter;
	// �Q�[�W�̊O���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_frame.png";
	parameter.name = "HealthGaugeFrame";
	parameter.position = { 10, 5 };
	parameter.charactor = charactor;
	UIData* health_gage_freame_data = new UIData(parameter);

	// �Q�[�W�̉��n
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_base.png";
	parameter.name = "HealthGaugeBase";
	parameter.position = { 22, 6 };
	UIData* health_gage_base_data = new UIData(parameter);

	// HP�̃Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_bl.png";
	parameter.name = "HealthGauge";
	parameter.position = { 23, 7 };
	UIData* health_green_gage_data = new UIData(parameter);

	// Hp�Q�[�W�̌������������Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_rd.png";
	parameter.name = "HealthSubtructGauge";
	parameter.position = { 23, 7 };
	UIData* health_red_gage_data = new UIData(parameter);

	ui_node_tree = new UINodeTree();

	// UI�ݒ�
	ui_node_tree->AddNode("", "root", NULL, NULL);
	ui_node_tree->AddNode("root", "Health", NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthFreame", health_gage_freame_data, NULL);
	ui_node_tree->AddNode("HealthFreame", "HealthFreame2", health_gage_base_data, NULL);
	ui_node_tree->AddNode("Health", "HealthRedGage", health_red_gage_data, new PlayerRedHealthAction(health_red_gage_data));
	ui_node_tree->AddNode("Health", "HealthGreenGage", health_green_gage_data, new PlayerGreenHealthAction(health_green_gage_data));
}