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

// �J�������[�V�������
struct CameraMotionData
{
	float time;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 target;
};

namespace MessageData
{
	// �t���[�J����
	static	const	char* CameraChangeFreeMode = "CameraChangeFreeMode";
	struct	CameraChangeFreeModeData
	{
		DirectX::XMFLOAT3	target;
	};

	// ���b�N�I���J����
	static	const	char* CameraChangeLockonMode = "CameraChangeLockonMode";
	struct	CameraChangeLockonModeData
	{
		DirectX::XMFLOAT3	start;
		DirectX::XMFLOAT3	target;
	};

	// �G���A���J����
	static	const	char* CameraChangeArielMode = "CameraChangeArielMode";
	struct	CameraChangeArielModeData
	{
		DirectX::XMFLOAT3	start;
		DirectX::XMFLOAT3	target;
		DirectX::XMFLOAT3	lockon_target;
	};

	// ���[�V�����J����
	static	const	char* CameraChangeMotionMode = "CameraChangeMotionMode";
	struct	CameraChangeMotionModeData
	{
		std::vector<CameraMotionData>	data;
	};

	// ���j���[�I�[�v�����
	static const char* MENUOPENEVENT = "MenuOpen";
	struct MenuOpenData
	{
		MenuId	menu_id;
	};
	// ���j���[�N���[�Y���
	static	const	char* MENUCLOSEEVENT = "MENU CLOSE";
	struct MenuCloseData
	{
		int	dummy;
	};

};