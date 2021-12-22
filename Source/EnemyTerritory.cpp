#include "Graphics.h"

#include "EnemyTerritory.h"
#include "DebugRenderer.h"

//*********************************************
// 
// �G�̓꒣��N���X
// 
//*********************************************
//---------------------------------------------
// �꒣��͈͂̃f�o�b�N�v���~�e�B�u�`��
//---------------------------------------------
void EnemyTerritory::DebugRenderTerritory()
{
	DebugRenderer* renderer = Graphics::Instance().GetDebugRenderer();
	// �꒣��͈͂̃f�o�b�O�~���`��
	renderer->DrawCylinder(territory_origin, territory_range, debug_cylinder, DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
}
