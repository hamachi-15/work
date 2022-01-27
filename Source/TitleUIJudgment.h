#pragma once
#include "UIJudgmentBase.h"


//*********************************************
// 
// スタートボタンの選択時描画を行うかの判定クラス
// 
//*********************************************
class StartButtonSelectDrawJudgment : public UIJudgmentBase
{
public:
	// コンストラクタ
	StartButtonSelectDrawJudgment(UIData * data) : UIJudgmentBase(data) {}
	
	// 判定
	bool Judgment() override;
};

//***********************************************
// 
// スタートボタンの非選択時描画を行うかの判定クラス
// 
//***********************************************
class StartButtonUnselectDrawJudgment : public UIJudgmentBase
{
public:
	// コンストラクタ
	StartButtonUnselectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// 判定
	bool Judgment() override;
};

//*****************************************
// 
// 操作ボタンの選択時描画を行うかの判定クラス
// 
//*****************************************
class OperateButtonSelectDrawJudgment : public UIJudgmentBase
{
public:
	// コンストラクタ
	OperateButtonSelectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// 判定
	bool Judgment() override;
};

//********************************************
// 
// 操作ボタンの非選択時描画を行うかの判定クラス
// 
//********************************************
class OperateButtonUnselectDrawJudgment : public UIJudgmentBase
{
public:
	// コンストラクタ
	OperateButtonUnselectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// 判定
	bool Judgment() override;
};