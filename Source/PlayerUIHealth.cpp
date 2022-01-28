#include "PlayerUIHealth.h"
#include "UINodeTree.h"
#include "PlayerUIAction.h"
#include "UIData.h"
#include "ActorManager.h"
#include "Charactor.h"

//-------------------------------
// �J�n����
//-------------------------------
void PlayerHealthUI::Start()
{
	std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
	UIParameter parameter;
	// �Q�[�W�̊O���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_frame.png";
	parameter.name = "HealthGaugeFrame";
	parameter.position = { 10, 650 };
	parameter.scale = { 1.0f, 1.2f };

	parameter.charactor = charactor;
	UIData* health_gauge_freame_data = new UIData(parameter);

	// �Q�[�W�̉��n
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_base.png";
	parameter.name = "HealthGaugeBase";
	parameter.position = { 22, 651 };
	UIData* health_gauge_base_data = new UIData(parameter);

	// HP�̃Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_bl.png";
	parameter.name = "HealthGauge";
	parameter.position = { 23, 652 };
	UIData* health_green_gauge_data = new UIData(parameter);

	// Hp�Q�[�W�̌������������Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_rd.png";
	parameter.name = "HealthSubtructGauge";
	parameter.position = { 23, 652 };
	UIData* health_red_gauge_data = new UIData(parameter);

	ui_node_tree = new UINodeTree();

	// UI�ݒ�
	ui_node_tree->AddNode("", "root", NULL, NULL, NULL);
	ui_node_tree->AddNode("root", "Health", NULL, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthFreame", health_gauge_base_data, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthRedGauge", health_red_gauge_data, NULL, new PlayerRedHealthAction(health_red_gauge_data));
	ui_node_tree->AddNode("Health", "HealthGreenGauge", health_green_gauge_data, NULL, new PlayerGreenHealthAction(health_green_gauge_data));
	ui_node_tree->AddNode("Health", "HealthFreame2", health_gauge_freame_data, NULL, NULL);
}