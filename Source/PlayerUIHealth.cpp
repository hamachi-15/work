#include "PlayerUIHealth.h"
#include "UINodeTree.h"
#include "PlayerUIAction.h"
#include "ActorManager.h"
#include "Charactor.h"

//-------------------------------
// 開始処理
//-------------------------------
void PlayerHealthUI::Start()
{
	std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
	UIParameter parameter;
	// ゲージの外側のデータ
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_frame.png";
	parameter.name = "HealthGaugeFrame";
	parameter.position = { 10, 650 };
	parameter.scale = { 1.0f, 1.2f };

	parameter.charactor = charactor;
	health_gauge_freame_data = std::make_unique<UIData>(parameter);

	// ゲージの下地
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_base.png";
	parameter.name = "HealthGaugeBase";
	parameter.position = { 22, 651 };
	health_gauge_base_data = std::make_unique<UIData>(parameter);

	// HPのゲージ
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_bl.png";
	parameter.name = "HealthGauge";
	parameter.position = { 23, 652 };
	health_green_gauge_data = std::make_unique<UIData>(parameter);

	// Hpゲージの減少幅を示すゲージ
	parameter.filename = "Data/Sprite/UIAseet/KP_clusterUI_v100/hp_gauge/gauge_rd.png";
	parameter.name = "HealthSubtructGauge";
	parameter.position = { 23, 652 };
	health_red_gauge_data = std::make_unique<UIData>(parameter);

	ui_node_tree = std::make_unique<UINodeTree>();

	// UI設定
	ui_node_tree->AddNode("", "root", NULL, NULL, NULL);
	ui_node_tree->AddNode("root", "Health", NULL, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthFreame", health_gauge_base_data.get(), NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthRedGauge", health_red_gauge_data.get(), NULL, new PlayerRedHealthAction(health_red_gauge_data.get()));
	ui_node_tree->AddNode("Health", "HealthGreenGauge", health_green_gauge_data.get(), NULL, new PlayerGreenHealthAction(health_green_gauge_data.get()));
	ui_node_tree->AddNode("Health", "HealthFreame2", health_gauge_freame_data.get(), NULL, NULL);
}