#pragma once
#include "Component.h"
#include "Collision.h"

class Stage : public Component
{
public:
	Stage() {}
	~Stage()override {}
	// 名前取得
	std::string GetName() const override { return "Stage"; }
	
	// 開始処理
	void Start() override {}
	
	// 更新処理
	void Update(float elapsed_time) override {}

	//bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

};