#pragma once
#include "MenuBase.h"
#include "Sprite.h"
#include "SpriteShader.h"
#include "Texture.h"

//--------------------------------
// 操作説明画面
//--------------------------------
class OperateDictionaryMenu : public MenuBase
{
public:
	// コンストラクタ・デストラクタ
	OperateDictionaryMenu();
	~OperateDictionaryMenu()override {}

	// 更新処理
	void Update(float elapsed_time)override;

	// 描画処理
	void Render(ID3D11DeviceContext* context) override;

private:
	std::unique_ptr<SpriteShader>	shader;
	std::unique_ptr<Sprite>			sprite;
	std::unique_ptr<Texture>		texture;
};