#pragma once
#include <memory>
#include <set>
#include "Telegram.h"

class Actor;
class Enemy;
class Player;
class Charactor;
class Scene;

class MetaAI
{
public:
	enum class Identity : int
	{
		Meta = 0,
		WorldMap = 1,	// ワールドマップのID
		BattleMap = 2,	// ワールドマップのID
		Collision = 3,	// コリジョンのIDは1
		Player = 4,		// プレイヤーIDは2~4
		Enemy = 5,		// エネミーIDは5以降を付与
		Slime,
		PLT,
		LAT,
		NightmearDragon,
		SoulEaterDragon
	};

	// コンストラクタ・デストラクタ
	MetaAI();
	~MetaAI(){}

	// インスタンス取得
	static MetaAI& Instance()
	{
		static MetaAI instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);
	
	// Enemy用のレシーブ処理を指定
	void Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram);

	// Player用のレシーブ処理を指定
	void Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram);

	// MetaAI用のレシーブ処理を指定
	void Discharge(const Telegram& telegram);

	//シーン用のレシーブ処理を指定
	void Discharge(Scene* receiver, const Telegram& telegram);

	// メッセージ受信したときの処理
	bool OnMessages(const Telegram& message);

	// メッセージ送信関数
	void SendMessaging(int sender, int receiver, Message message, int enemy_identity = 0);

	// 敵の出現処理
	void AppearanceEnemy();
private:
	std::set<Telegram>			priority;
};