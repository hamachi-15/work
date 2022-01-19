#pragma once
#include <DirectXMath.h>

enum	class	EnemyTerritoryTag;

enum class MessageType : int
{
	Message_Call_Help,				// 仲間を呼ぶ(Enemy、メタAIの両方で使用)
	Message_Change_Attack_Right,	// 攻撃権の移譲(EnemyからメタAIへのメッセージ用)
	Message_Give_Attack_Right,		// 攻撃権を与える（メタAIからEnemyへのメッセージ用）
	Message_Ask_Attack_Right,		// 攻撃権を要求(EnemyからメタAIへのメッセージ用)
	Message_Hit_Boddy,				// 体コリジョン同士が当たった(Enemy,Playerで使用)
	Message_Hit_Attack,				// 攻撃が当たった(Enemy,Playerで使用)
	Message_GetHit_Attack			// 攻撃を受けた(Enemy,Playerで使用)
};

struct Message
{
	MessageType			message;	// メッセージID
	DirectX::XMFLOAT3	hit_position = {0.0f, 0.0f, 0.0f}; // 
	EnemyTerritoryTag territory_tag;
};

class Telegram
{	
public:
	// コンストラクタ
	Telegram(int sender, int receiver, Message message);
		
	int				sender;				// 送り手のID
	int				receiver;			// 受け手のID
	Message			message_box;
};