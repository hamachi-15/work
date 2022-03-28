#pragma once
#include "Scene.h"

class SceneTutorial : public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial()override{}
private:
	// ‰Šú‰»ˆ—
	void Initialize()override;

	// I—¹‰»ˆ—
	void Finalize()override;

	// XVˆ—
	void Update(float elapsed_time)override;

	// •`‰æˆ—
	void Render()override;

};