#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>

enum class MenuId
{
	None,
	TopMenu,
	OperateDictionaryMenu,
	TitleBackDictionaryMenu
};

// カメラモーション情報
struct CameraMotionData
{
	float time;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 target;
};

namespace MessageData
{
	// フリーカメラ
	static	const	char* CameraChangeFreeMode = "CameraChangeFreeMode";
	struct	CameraChangeFreeModeData
	{
		DirectX::XMFLOAT3	target;
	};

	// ロックオンカメラ
	static	const	char* CameraChangeLockonMode = "CameraChangeLockonMode";
	struct	CameraChangeLockonModeData
	{
		DirectX::XMFLOAT3	start;
		DirectX::XMFLOAT3	target;
	};

	// エリアルカメラ
	static	const	char* CameraChangeArielMode = "CameraChangeArielMode";
	struct	CameraChangeArielModeData
	{
		DirectX::XMFLOAT3	start;
		DirectX::XMFLOAT3	target;
		DirectX::XMFLOAT3	lockon_target;
	};

	// モーションカメラ
	static	const	char* CameraChangeMotionMode = "CameraChangeMotionMode";
	struct	CameraChangeMotionModeData
	{
		std::vector<CameraMotionData>	data;
	};

	// メニューオープン情報
	static const char* MENUOPENEVENT = "MenuOpen";
	struct MenuOpenData
	{
		MenuId	menu_id;
	};
	// メニュークローズ情報
	static	const	char* MENUCLOSEEVENT = "MENU CLOSE";
	struct MenuCloseData
	{
		int	dummy;
	};

};