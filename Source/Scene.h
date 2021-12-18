#pragma once
#include "Telegram.h"
#include <string>

class Scene
{
public:
	// コンストラクタ
	Scene(){}
	// デストラクタ
	virtual ~Scene(){}

	// シーン名設定
	void SetName(const char* scene_name) { this->scene_name = scene_name; }

	// シーン名取得
	const char* GetName() const { return scene_name.c_str(); }

	// 初期化処理
	virtual void Initialize() = 0;
	
	// 終了化処理
	virtual void Finalize() = 0;

	// 更新処理
	virtual void Update(float elapsed_time) = 0;
	
	// 描画処理
	virtual void Render() = 0;

	virtual bool OnMessages(const Telegram& telegram) { return true; }

	// 初期化完了
	bool IsReady() const { return ready; }
	void SetReady() { ready = true; }
private:
	bool ready = false;
	std::string scene_name; // シーンの名前
};