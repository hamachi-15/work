#pragma once
#include <DirectXMath.h>
#include "EnemyTerritoryTag.h"

class EnemyTerritory
{
public:
	// �R���X�g���N�^
	EnemyTerritory(){}
	// �f�X�g���N�^
	~EnemyTerritory() {}

	// �꒣��͈͂̃f�o�b�N�v���~�e�B�u�`��
	void DebugRenderTerritory();

	// �꒣��̃^�O�擾
	EnemyTerritoryTag GetTerritoryTag() const { return tag; }

	// �꒣��̃^�O�ݒ�
	void SetTerritoryTag(EnemyTerritoryTag& tag) { this->tag = tag; }

	// �꒣��̌��_�擾
	const DirectX::XMFLOAT3& GetTerritoryOrigin() const { return territory_origin; }
	
	// �꒣��̌��_�ݒ�
	void SetTerritoryOrigin(DirectX::XMFLOAT3& territory_origin) { this->territory_origin = territory_origin; }

	// �꒣��͈͎̔擾
	const float& GetTerritoryRange() const { return territory_range; }

	// �꒣��͈̔͐ݒ�
	void SetTerritoryRange(float& territory_range) { this->territory_range = territory_range; }
private:
	DirectX::XMFLOAT3	territory_origin;
	float				territory_range;
	float				debug_cylinder = 5.0f;	// �f�o�b�O�~���`��̍���
	EnemyTerritoryTag	tag;					// �����̓꒣�肩�����̂Ȃ�΂��T���p
};