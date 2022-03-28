#include <sstream>
#include "Framework.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "EffectManager.h"
#include "AudioManager.h"

// ���������Ԋu�ݒ�
static const int syncInterval = 1;

//-------------------------------
// �R���X�g���N�^
//-------------------------------
Framework::Framework(HWND hwnd) :
	hwnd(hwnd),
	input(hwnd),
	graphics(hwnd)
{
	// �V�[��������
	SceneManager::Instance().ChangeScene(new SceneTitle());
}

//-------------------------------
// �f�X�g���N�^
//-------------------------------
Framework::~Framework()
{
	// �t���X�N���[��������
	graphics.GetSwapChain()->SetFullscreenState(false, NULL);

	// �V�[���I������
	SceneManager::Instance().Clear();

	// �I�[�f�B�I�̔j������
	AudioManager::Instance().Destroy();
}

//-------------------------------
// �X�V����
//-------------------------------
void Framework::Update(float elapsed_time)
{
	// ���͍X�V����
	input.Update();

	// IMGUI�t���[���J�n����
	graphics.GetImGuiRenderer()->NewFrame();

	// �V�[���X�V����
	SceneManager::Instance().Update(elapsed_time);
}

//-------------------------------
// �`�揈��
//-------------------------------
void Framework::Render(float elapsed_time)
{
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// �~���[�e�N�X
	std::lock_guard<std::mutex> lock(graphics.GetMutex());

	// �V�[���`�揈��
	SceneManager::Instance().Render();

	// IMGUI�`��
	graphics.GetImGuiRenderer()->Render(dc);

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
	graphics.GetSwapChain()->Present(syncInterval, 0);
}

//-------------------------------
// fps�v�Z
//-------------------------------
void Framework::CalculateFrameStats()
{
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	++frames;

	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames);
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hwnd, outs.str().c_str());

		frames = 0;
		time_tlapsed += 1.0f;
	}
}

//-------------------------------
// ���b�Z�[�W���[�v
//-------------------------------
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();

			float elapsed_time = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / 60.0f
				;
			Update(elapsed_time);
			Render(elapsed_time);
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		timer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
