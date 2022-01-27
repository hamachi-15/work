#pragma once

class UIData;

class UIJudgmentBase
{
public:
	// コンストラクタ
	UIJudgmentBase(UIData* data) : data(data) {}

	// 判定処理
	virtual bool Judgment() = 0;
protected:
	UIData* data;
};