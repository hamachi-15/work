#pragma once
#include "Component.h"
#include "Collision.h"

class Stage : public Component
{
public:
	Stage() {}
	~Stage()override {}
	// ���O�擾
	std::string GetName() const override { return "Stage"; }
	
	// �J�n����
	void Start() override {}
	
	// �X�V����
	void Update(float elapsed_time) override {}

	//bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

};