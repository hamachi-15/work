#pragma once
#include "UIJudgmentBase.h"


//*********************************************
// 
// �X�^�[�g�{�^���̑I�����`����s�����̔���N���X
// 
//*********************************************
class StartButtonSelectDrawJudgment : public UIJudgmentBase
{
public:
	// �R���X�g���N�^
	StartButtonSelectDrawJudgment(UIData * data) : UIJudgmentBase(data) {}
	
	// ����
	bool Judgment() override;
};

//***********************************************
// 
// �X�^�[�g�{�^���̔�I�����`����s�����̔���N���X
// 
//***********************************************
class StartButtonUnselectDrawJudgment : public UIJudgmentBase
{
public:
	// �R���X�g���N�^
	StartButtonUnselectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// ����
	bool Judgment() override;
};

//*****************************************
// 
// ����{�^���̑I�����`����s�����̔���N���X
// 
//*****************************************
class OperateButtonSelectDrawJudgment : public UIJudgmentBase
{
public:
	// �R���X�g���N�^
	OperateButtonSelectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// ����
	bool Judgment() override;
};

//********************************************
// 
// ����{�^���̔�I�����`����s�����̔���N���X
// 
//********************************************
class OperateButtonUnselectDrawJudgment : public UIJudgmentBase
{
public:
	// �R���X�g���N�^
	OperateButtonUnselectDrawJudgment(UIData* data) : UIJudgmentBase(data) {}

	// ����
	bool Judgment() override;
};