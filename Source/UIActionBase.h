#pragma once
// 前方宣言
class UIData;

//****************************
// 
// UIベースクラス
// 
//****************************
class UIActionBase
{
public:
	// コンストラクタ
	UIActionBase(UIData* ui_data) : ui_data(ui_data) {}

	// アクション(純粋仮想関数)
	virtual void Action(float elapsed_time) = 0;

protected:
	UIData* ui_data;
};