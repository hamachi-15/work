#pragma once

class UIData;

class UIJudgmentBase
{
public:
	// �R���X�g���N�^
	UIJudgmentBase(UIData* data) : data(data) {}

	// ���菈��
	virtual bool Judgment() = 0;
protected:
	UIData* data;
};