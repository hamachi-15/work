#pragma once
#include "Scene.h"

class SceneTutorial : public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override{}
private:
	// ����������
	void Initialize()override;

	// �I��������
	void Finalize()override;

	// �X�V����
	void Update(float elapsed_time)override;

	// �`�揈��
	void Render()override;

};