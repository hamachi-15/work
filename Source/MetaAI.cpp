
#include "Mathf.h"

#include "MetaAI.h"

#include "SceneManager.h"

#include "Charactor.h"
#include "ActorManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"

//-----------------------------------
// コンストラクタ
//-----------------------------------
MetaAI::MetaAI()
{	
}

//-----------------------------------
// 更新処理
//-----------------------------------
void MetaAI::Update(float elapsed_time)
{
	// プレイヤーが死なずに敵を一定数倒せば敵のステータスを強化させる
	// 一定時間内のプレイヤーへのダメージ回数が特定回数以上なら敵のステータスを弱化させる
}

//-----------------------------------
// プレイヤーへメッセージを飛ばす
//-----------------------------------
void MetaAI::Discharge(std::shared_ptr<Player> receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// 受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// 敵へメッセージを飛ばす
//-----------------------------------
void MetaAI::Discharge(std::shared_ptr<Enemy> receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// 受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// MetaAI用のレシーブ処理を指定
//-----------------------------------
void MetaAI::Discharge(const Telegram& telegram)
{
	if (!OnMessages(telegram))
	{
		// 受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}

//-----------------------------------
// シーンへ送る用のレシーブ処理を指定
//-----------------------------------
void MetaAI::Discharge(Scene* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessages(telegram))
	{
		// 受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Scene Receive Failed");
	}
}

//-----------------------------------
// メッセージ受信したときの処理
//-----------------------------------
bool MetaAI::OnMessages(const Telegram& telegram)
{
	switch (telegram.message_box.message)
	{
	case MessageType::Message_Ask_Attack_Right:
	{
		int count = 0;// 全ての敵について、攻撃権を持っていないか調べる。
		int enemy_count = EnemyManager::Instance().GetEnemyCount();
		for (int i = 0; i < enemy_count; ++i)
		{
			std::shared_ptr<Enemy> enemy = EnemyManager::Instance().GetEnemy(i);
			if (enemy->GetAttackFlag()) count++;
		}
		// 他に攻撃権持っている敵がいないなら、要求してきた敵に攻撃権を与えるメッセージを送る。
		if (count < 1)
		{
			// 誰も攻撃権を持っていなければMetaAIから送信者に攻撃権付与のメッセージを送る
			Message message_box;
			message_box.message = MessageType::Message_Give_Attack_Right;
			SendMessaging(static_cast<int>(MetaAI::Identity::Meta), telegram.sender, message_box);
		}
	}
		break;
	case MessageType::Message_Hit_Attack:
		
		break;
	case MessageType::Message_Hit_Boddy:
		break;
	}
	return false;
}

//-----------------------------------
// メッセージ送信関数
//-----------------------------------
void MetaAI::SendMessaging(int sender, int receiver, Message message, int enemy_identity)
{
	EnemyManager& enemy_manager = EnemyManager::Instance();
	if (receiver == static_cast<int>(MetaAI::Identity::Meta))
	{// MetaAI宛ての時
		Telegram telegram(sender, receiver, message);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}
	else if(receiver == static_cast<int>(MetaAI::Identity::WorldMap))
	{// 
		Telegram telegram(sender, receiver, message);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(SceneManager::Instance().GetCurrentScene(), telegram);
	}
	else if (receiver == static_cast<int>(MetaAI::Identity::BattleMap))
	{
		Telegram telegram(sender, receiver, message);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(SceneManager::Instance().GetCurrentScene(), telegram);
	}
	else if(receiver == static_cast<int>(MetaAI::Identity::Player))
	{// プレイヤーが受信者
		// プレイヤーコンポーネントを取得
		std::shared_ptr<Player> player = ActorManager::Instance().GetActor("Player")->GetComponent<Player>();
		// メッセージデータを作成
		Telegram telegram(sender, receiver, message);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(player, telegram);
	}
	else // プレイヤーとメタ以外は敵
	{// 敵が受信者の場合どの敵に送信するか
		// レシーバーのポインタを取得
		std::shared_ptr<Enemy> receive_enemy = enemy_manager.GetEnemyFromID(receiver);
		// レシーバーがいなければ終了
		if (receive_enemy == nullptr) return;
		// メッセージデータを作成
		Telegram telegram(sender, receiver, message);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_enemy, telegram);
	}
}

//-----------------------------------
// 敵の出現処理
//-----------------------------------
void MetaAI::AppearanceEnemy()
{
	// 敵の縄張り設定
	EnemyManager::Instance().CreateTerritory();
	// 敵を生成
	EnemyManager::Instance().CreateEnemies();
}
