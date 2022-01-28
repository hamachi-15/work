#include "BossHealthUI.h"
#include "BossHealthUIAction.h"
#include "PlayerUIAction.h"
#include "UINodeTree.h"
#include "UIData.h"
#include "ActorManager.h"
#include "Charactor.h"

//**********************************
// 
// �{�X�N���X�̓GHPUI�N���X
// 
//**********************************
//-------------------------
// �J�n����
//-------------------------
void BossHealthUI::Start()
{
	std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
	UIParameter parameter;
	// �Q�[�W�̊O���̃f�[�^
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GaugeCover.png";
	parameter.name = "HealthGaugeFrame";
	parameter.position = { 175, 20 };
	parameter.charactor = charactor;
	parameter.scale = { 0.5f, 0.25f };
	UIData* health_gage_freame_data = new UIData(parameter);

	// �Q�[�W�̉��n
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GaugeBase.png";
	parameter.name = "HealthGaugeBase";
	parameter.position = { 90, 25 };
	UIData* health_base_data = new UIData(parameter);

	// HP�̃Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GoldGauge.png";
	parameter.name = "HealthGauge";
	parameter.position = { 220, 30 };
	UIData* health_gold_gage_data = new UIData(parameter);

	// Hp�Q�[�W�̌������������Q�[�W
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/RedGauge.png";
	parameter.name = "HealthSubtructGauge";
	UIData* health_red_gage_data = new UIData(parameter);

	ui_node_tree = new UINodeTree();

	// UI�ݒ�
	ui_node_tree->AddNode("", "root", NULL, NULL, NULL);
	ui_node_tree->AddNode("root", "Health", NULL, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthBase", health_base_data, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthRedGauge", health_red_gage_data, NULL, new PlayerRedHealthAction(health_red_gage_data));
	ui_node_tree->AddNode("Health", "HealthGoldGauge", health_gold_gage_data, NULL, new PlayerGreenHealthAction(health_gold_gage_data));
	ui_node_tree->AddNode("Health", "HealthFreame", health_gage_freame_data, NULL, NULL);
}
