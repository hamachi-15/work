#pragma once
#include "UI.h"

class PlayerUIHealthFrame : public UI
{
public:
	// �R���X�g���N�^
	PlayerUIHealthFrame(const UIParameter& parameter) : UI(parameter){}

	// �f�X�g���N�^
	~PlayerUIHealthFrame();
	
	// �X�V����
	void Update(float elapsed_time);
};