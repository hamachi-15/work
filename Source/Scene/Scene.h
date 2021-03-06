#pragma once
#include "Telegram.h"
#include "CameraController.h"
#include <string>
#include <memory>

class Scene
{
public:
	// コンストラクタ
	Scene(){}

	// デストラクタ
	virtual ~Scene(){}

	// シーン名取得
	virtual std::string GetName() const = 0;

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

	// シーンチェンジフラグ
	void SetSceneChangeFlag() { change_scene_flag = true; }
	bool IsSceneChangeFlag() const { return change_scene_flag; }

	// カメラコントローラー取得
	CameraController* GetCameraController() const { return camera_controller.get(); }
private:
	bool ready = false;
	std::string scene_name; // シーンの名前
public: // TDDO
	std::unique_ptr<CameraController>	camera_controller;
	bool change_scene_flag = false;
};