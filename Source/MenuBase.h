#pragma once
#include "Model.h"
#include "Shader.h"

// メニューの基底クラス
class MenuBase
{
public:
	MenuBase(){}
	virtual~MenuBase(){}

	virtual void Update(float elapsed_time) = 0;
	virtual void Render(ID3D11DeviceContext* context) = 0;
};