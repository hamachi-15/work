#include "Graphics.h"

#include "EnemyTerritory.h"
#include "DebugRenderer.h"

//*********************************************
// 
// 敵の縄張りクラス
// 
//*********************************************
//---------------------------------------------
// 縄張り範囲のデバックプリミティブ描画
//---------------------------------------------
void EnemyTerritory::DebugRenderTerritory()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	// 縄張り範囲のデバッグ円柱描画
	renderer->DrawCylinder(territory_origin, territory_range, debug_cylinder, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
}
