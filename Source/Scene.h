#pragma once

class Scene
{
public:
	// コンストラクタ
	Scene(){}
	// デストラクタ
	virtual ~Scene(){}

	// 初期化処理
	virtual void Initialize() = 0;
	
	// 終了化処理
	virtual void Finalize() = 0;

	// 更新処理
	virtual void Update(float elapsed_time) = 0;
	
	// 描画処理
	virtual void Render() = 0;

	// 初期化完了
	bool IsReady() const { return ready; };
	void SetReady() { ready = true; }
private:
	bool ready = false;
};