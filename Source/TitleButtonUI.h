#pragma once
#include "UI.h"
#include "UIData.h"

//********************************
// 
// タイトルのボタンUI
// 
//********************************
class TitleButtonUI : public UI
{
public:
	enum class SelectState;
	enum class ButtonType;
public:
	// コンストラクタ
	TitleButtonUI() {}

	// デストラクタ
	~TitleButtonUI() override;

	// 名前取得
	std::string GetName() const override { return "TitleButtonUI"; }

	// 開始処理
	void Start() override;

	// ボタンの選択状態設定
	void SetSelectState(ButtonType index, SelectState select_state) { select_button[static_cast<int>(index)] = select_state; }

	// ボタンの選択状態取得
	SelectState GetButtonState(ButtonType index)const { return select_button[static_cast<int>(index)]; }
public:
	enum class SelectState
	{
		Unselect_Game_Start,
		Select_Game_Start,
	};
	enum class ButtonType
	{
		Start_Button,
		Operate_Button,
		Button_Max
	};
private:
	SelectState select_button[static_cast<int>(ButtonType::Button_Max)];

	UIData* start_button_data;
	UIData* select_start_button_data;
	UIData* start_data;

	UIData* operate_button_data;
	UIData* select_operate_button_data;
	UIData* operate_data;
};