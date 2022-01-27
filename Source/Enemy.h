#pragma once
#include "Component.h"
#include "Movement.h"

enum class EnemyTerritoryTag;
class BehaviorTree;
class BehaviorData;
class NodeBase;
class Telegram;
class Charactor;

class Enemy : public Component
{
public:
	// コンストラクタ
	Enemy();
	// デストラクタ
	~Enemy() override;

	// 名前取得
	std::string GetName() const override { return name.c_str(); }
	
	// 名前設定
	void SetName(const char* name) { this->name = name; }
	
	// GUI描画
	void OnGUI() override {};
	
	// 開始処理
	void Start() override {};
	
	// 更新処理
	void Update(float elapsed_time) override {};
	
	// ビヘイビア更新処理
	void BehaviorUpdate(float elapsed_time);

	// 当たり範囲デバッグプリミティブ描画
	virtual void DrawDebugPrimitive(){}
	
	// 敵の破棄処理
	virtual void Destroy() = 0;

	// エネミーデータID設定
	void SetEnemyDataID(int id) { this->id = id; }

	// エネミーデータID取得
	int GetEnemyDataID() const { return id; }

	// アイデンティティ設定
	void SetIdentity(int identity) { this->identity = identity; }
	
	// アイデンティティ取得
	int GetIdentity() { return this->identity; }
	
	// ビヘイビアのノード設定処理
	virtual void SetBehaviorNode() = 0;

	// ムーブメントの設定
	void SetMovement(std::shared_ptr<Movement> movement) { this->movement = movement; }
	
	// ムーブメントの取得
	std::shared_ptr<Movement> GetMovement() const { return movement; }
	
	// キャラクターの設定
	void SetCharactor(std::shared_ptr<Charactor> charactor) { this->charactor = charactor; }
	
	// キャラクターの取得
	std::shared_ptr<Charactor> GetCharactor() const { return charactor; }

	// 範囲内にプレイヤーがいないか探す
	bool SearchPlayer();
	
	// 目的地点へ移動
	void MoveToTarget(float elapsed_time, float speed_rate);
	
	// ターゲット位置をランダム設定
	void SetRandomTargetPosition();
	
	// ターゲット位置を設定
	void SetTargetPosition(const DirectX::XMFLOAT3& target_position) { this->target_position = target_position; }
	
	// ターゲット位置を取得
	DirectX::XMFLOAT3 GetTargetPosition() const { return target_position; }
	
	// プレイヤーの攻撃コリジョンの位置設定
	void SetHitPosition(const DirectX::XMFLOAT3& hit_position) { this->hit_position = hit_position; }

	// プレイヤーの攻撃コリジョンの位置取得
	const DirectX::XMFLOAT3& GetHitPosition() const { return hit_position; }

	// テリトリータグ設定
	void SetBelongingToTerritory(EnemyTerritoryTag teritory_tag) { this->teritory_tag = teritory_tag; }

	// テリトリータグ取得
	const EnemyTerritoryTag& GetBelongingToTerritory() const { return teritory_tag; }
	
	// 攻撃範囲設定
	void SetAttackRange(const float& attack_range) { this->attack_range = attack_range; }

	// 攻撃範囲取得
	float GetAttackRange() const { return attack_range; }

	// 索敵範囲設定
	void SetSearchRange(const float search_range) { this->search_range = search_range; }

	// 攻撃フラグ設定
	void SetAttackFlag(bool attack_flag) { this->attack_flag = attack_flag; }
	
	// 攻撃フラグ取得
	const bool& GetAttackFlag() const { return attack_flag; }
	
	// 怒り状態フラグ設定
	void SetAngerFlag(bool anger_flag) { this->anger_flag = anger_flag; }

	// 怒り状態フラグ取得
	const bool& GetAngerFlag() const { return anger_flag; }

	float GetRunTimer() const { return run_timer; }

	void SetRunTimer(const float& run_timer) { this->run_timer = run_timer; }

	// アニメーション再生
	virtual void PlayAnimation(const char* animation_name);

	// ダメージ処理
	void OnDamaged();

	// 死亡処理
	void OnDead();

	// メッセージを受信したときの処理
	virtual bool OnMessages(const Telegram& message) = 0;

	// ビヘイビアのGUI描画
	void DrawBehaviorGUI();

	// 
private:
	int							id;
	int							identity;			// 何番目の敵か
	bool						attack_flag = false;		// 攻撃フラグ
	bool						anger_flag = false;				 // 怒り状態フラグ
	std::shared_ptr<Movement>	movement;
	std::shared_ptr<Charactor>	charactor;
	EnemyTerritoryTag			teritory_tag;
protected:
	DirectX::XMFLOAT3	target_position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	hit_position = { 0.0f, 0.0f, 0.0f };	// プレイヤーの攻撃コリジョンの位置(自身と衝突したときに使う)
	std::string			name;
	BehaviorTree*		ai_tree = nullptr;
	BehaviorData*		behavior_data = nullptr;
	NodeBase*			active_node = nullptr;
	NodeBase*			old_active_node = nullptr;
	float				run_timer = 0.0f;
	//float				territory_range = 10.0f; // 縄張りの範囲
	//DirectX::XMFLOAT3	territory_origin = { 0.0f,0.0f,0.0f };
	float				search_range = 5.0f;	 // 索敵範囲
	float				attack_range = 1.5f;	 // 攻撃に移る範囲
};

