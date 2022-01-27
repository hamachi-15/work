#pragma once
#include <string>
#include "Actor.h"

//**********************************
// 
// コンポーネント取得
// 
//**********************************
class Component
{
public:
	// コンストラクタ
	Component() {}

	// デストラクタ
	virtual ~Component() {}

	// 名前取得
	virtual std::string GetName() const = 0;

	// 開始処理
	virtual void Start() {}

	// 更新処理
	virtual void Update(float elapsed_time) {}

	// GUI描画
	virtual void OnGUI() {}

	// アクター設定
	void SetActor(std::shared_ptr<Actor> actor) { this->actor = actor; }

	// アクター取得
	std::shared_ptr<Actor> GetActor() { return actor.lock(); }
private:
	std::weak_ptr<Actor> actor;
};