#include "BossHealthUI.h"
#include "BossHealthUIAction.h"
#include "PlayerUIAction.h"
#include "UINodeTree.h"
#include "ActorManager.h"
#include "Charactor.h"

//**********************************
// 
// ボスクラスの敵HPUIクラス
// 
//**********************************
//-------------------------
// 開始処理
//-------------------------
void BossHealthUI::Start()
{
	std::shared_ptr<Charactor> charactor = GetActor()->GetComponent<Charactor>();
	UIParameter parameter;
	// ゲージの外側のデータ
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GaugeCover.png";
	parameter.name = "HealthGaugeFrame";
	parameter.position = { 175, 20 };
	parameter.charactor = charactor;
	parameter.scale = { 0.5f, 0.25f };
	health_gage_freame_data = std::make_unique<UIData>(parameter);

	// ゲージの下地
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GaugeBase.png";
	parameter.name = "HealthGaugeBase";
	parameter.position = { 90, 25 };
	health_base_data = std::make_unique<UIData>(parameter);

	// HPのゲージ
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/GoldGauge.png";
	parameter.name = "HealthGauge";
	parameter.position = { 220, 30 };
	health_gold_gage_data = std::make_unique<UIData>(parameter);

	// Hpゲージの減少幅を示すゲージ
	parameter.filename = "Data/Sprite/UIAseet/DragonHealthGauge/RedGauge.png";
	parameter.name = "HealthSubtructGauge";
	health_red_gage_data = std::make_unique<UIData>(parameter);

	ui_node_tree = std::make_unique<UINodeTree>();

	// UI設定
	ui_node_tree->AddNode("", "root", NULL, NULL, NULL);
	ui_node_tree->AddNode("root", "Health", NULL, NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthBase", health_base_data.get(), NULL, NULL);
	ui_node_tree->AddNode("Health", "HealthRedGauge", health_red_gage_data.get(), NULL, new PlayerRedHealthAction(health_red_gage_data.get()));
	ui_node_tree->AddNode("Health", "HealthGoldGauge", health_gold_gage_data.get(), NULL, new PlayerGreenHealthAction(health_gold_gage_data.get()));
	ui_node_tree->AddNode("Health", "HealthFreame", health_gage_freame_data.get(), NULL, NULL);
}
