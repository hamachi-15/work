#pragma once
#include <memory>
#include <set>
#include "Telegram.h"

class Actor;
class Enemy;
class Player;
class Charactor;

class MetaAI
{
public:
	enum class Identity : int
	{
		Meta = 0,
		Collision = 1,	// コリジョンのIDは1
		Player = 2,		// プレイヤーIDは2~4
		Enemy = 5		// エネミーIDは5以降を付与
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
	
	// このメソッドはtelegramクラスを作成して
	// Enemyを継承するクラスのメッセージレシーバーの処理を呼び出す。
	//void Discharge(Enemy* receiver, const Telegram& telegram);
	void Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram);
	void Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram);

	// MetaAI用のレシーブ処理を指定
	void Discharge(const Telegram& telegram);

	// メッセージ受信したときの処理
	bool OnMessages(const Telegram& message);

	// メッセージ送信関数
	void SendMessaging(int sender, int receiver, Message message, int enemy_identity = 0);

	// 敵の出現処理
	void AppearanceEnemy(float elapsed_time);
private:
	std::set<Telegram>			priority;
	std::shared_ptr<Player>		player;

	float						pop_interval;
	int index;
};