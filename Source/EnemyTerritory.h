#pragma once
#include <DirectXMath.h>
#include "EnemyTerritoryTag.h"

class EnemyTerritory
{
public:
	// コンストラクタ
	EnemyTerritory(){}
	// デストラクタ
	~EnemyTerritory() {}

	// 縄張り範囲のデバックプリミティブ描画
	void DebugRenderTerritory();

	// 縄張りのタグ取得
	EnemyTerritoryTag GetTerritoryTag() const { return tag; }

	// 縄張りのタグ設定
	void SetTerritoryTag(EnemyTerritoryTag& tag) { this->tag = tag; }

	// 縄張りの原点取得
	const DirectX::XMFLOAT3& GetTerritoryOrigin() const { return territory_origin; }
	
	// 縄張りの原点設定
	void SetTerritoryOrigin(DirectX::XMFLOAT3& territory_origin) { this->territory_origin = territory_origin; }

	// 縄張りの範囲取得
	const float& GetTerritoryRange() const { return territory_range; }

	// 縄張りの範囲設定
	void SetTerritoryRange(float& territory_range) { this->territory_range = territory_range; }
private:
	DirectX::XMFLOAT3	territory_origin;
	float				territory_range;
	float				debug_cylinder = 5.0f;	// デバッグ円柱描画の高さ
	EnemyTerritoryTag	tag;					// 複数の縄張りから特定のなわばりを探す用
};