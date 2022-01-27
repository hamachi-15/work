#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include <string>

class UI;
//******************************
// 
// UIマネージャー
// 
//******************************
class UIManager
{
private:
	// コンストラクタ
	UIManager();
	// デストラクタ
	~UIManager();
public:
	// インスタンス取得
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Draw(ID3D11DeviceContext* context);

	// UI登録
	void RegisterUI(std::shared_ptr<UI> ui);

	// UI削除
	void UnRegisterUI(std::shared_ptr<UI> ui);

	// 全削除
	void AllDelete();

	// UI取得
	std::shared_ptr<UI> GetUI(std::string name);
private:
	std::vector<std::shared_ptr<UI>> ui_starts;
	std::vector<std::shared_ptr<UI>> ui_updates;
	std::vector<std::shared_ptr<UI>> ui_removes;
};