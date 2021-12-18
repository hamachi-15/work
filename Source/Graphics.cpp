#include "Misc.h"
#include "Graphics.h"
#include "SpriteShader.h"
#include "SkyBoxShader.h"
#include "Audio.h"

Graphics* Graphics::instance = nullptr;


//-----------------------------
// �R���X�g���N�^
//-----------------------------
Graphics::Graphics(HWND hwnd)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	// ��ʂ̃T�C�Y���擾����B
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT screen_width = rc.right - rc.left;
	UINT screen_height = rc.bottom - rc.top;

	this->screen_width = static_cast<float>(screen_width);
	this->screen_height = static_cast<float>(screen_height);

	HRESULT hr = S_OK;

	// �f�o�C�X���X���b�v�`�F�[���̐���
	{
		UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL feature_levels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// �X���b�v�`�F�[�����쐬���邽�߂̐ݒ�I�v�V����
		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		{
			swapchain_desc.BufferDesc.Width = screen_width;
			swapchain_desc.BufferDesc.Height = screen_height;
			swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 1�s�N�Z��������̊e�F(RGBA)��8bit(0�`255)�̃e�N�X�`��(�o�b�N�o�b�t�@)���쐬����B
			swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = 1;		// �o�b�N�o�b�t�@�̐�
			swapchain_desc.OutputWindow = hwnd;	// DirectX�ŕ`�������\������E�C���h�E
			swapchain_desc.Windowed = TRUE;		// �E�C���h�E���[�h���A�t���X�N���[���ɂ��邩�B
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchain_desc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		}	

		D3D_FEATURE_LEVEL feature_level;

		// �f�o�C�X���X���b�v�`�F�[���̐���
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,						// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr�ŁAIDXGIAdapter�̃A�h���X��n���B
			D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���BD3D_DRIVER_TYPE_HARDWARE �ȊO�͊�{�I�Ƀ\�t�g�E�F�A�����ŁA���ʂȂ��Ƃ�����ꍇ�ɗp����B
			nullptr,						// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵���ۂɁA���̏������s��DLL�̃n���h����n���B����ȊO���w�肵�Ă���ۂɂ͕K��nullptr��n���B
			create_device_flags,			// ���炩�̃t���O���w�肷��B�ڂ�����D3D11_CREATE_DEVICE�񋓌^�Ō����B
			feature_levels,					// D3D_FEATURE_LEVEL�񋓌^�̔z���^����Bnullptr�ɂ��邱�Ƃł���Lfeature�Ɠ����̓��e�̔z�񂪎g�p�����B
			ARRAYSIZE(feature_levels),		// featureLevels�z��̗v�f����n���B
			D3D11_SDK_VERSION,				// SDK�̃o�[�W�����B�K�����̒l�B
			&swapchain_desc,				// �����Őݒ肵���\���̂ɐݒ肳��Ă���p�����[�^��SwapChain���쐬�����B
			swapchain.GetAddressOf(),		// �쐬�����������ꍇ�ɁASwapChain�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��SwapChain�𑀍삷��B
			device.GetAddressOf(),			// �쐬�����������ꍇ�ɁADevice�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Device�𑀍삷��B
			&feature_level,					// �쐬�ɐ�������D3D_FEATURE_LEVEL���i�[���邽�߂�D3D_FEATURE_LEVEL�񋓌^�ϐ��̃A�h���X��ݒ肷��B
			context.GetAddressOf()			// �쐬�����������ꍇ�ɁAContext�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��Context�𑀍삷��B
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}


	// �[�x�X�e���V���r���[�̐���
	{
		// �[�x�X�e���V�������������ނ��߂̃e�N�X�`�����쐬����B
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = screen_width;
		depthStencilBufferDesc.Height = screen_height;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 1�s�N�Z��������A�[�x����24Bit / �X�e���V������8bit�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// �[�x�X�e���V���p�̃e�N�X�`�����쐬����B
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depthStencilBufferDesc, nullptr, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


		// �[�x�X�e���V���e�N�X�`���ւ̏������݂ɑ����ɂȂ�[�x�X�e���V���r���[���쐬����B
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �����_�[�^�[�Q�b�g�r���[�̐���
	{
		// �X���b�v�`�F�[������o�b�N�o�b�t�@�e�N�X�`�����擾����B
		// ���X���b�v�`�F�[���ɓ����Ă���o�b�N�o�b�t�@�e�N�X�`����'�F'���������ރe�N�X�`���B
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �o�b�N�o�b�t�@�e�N�X�`���ւ̏������݂̑����ƂȂ郌���_�[�^�[�Q�b�g�r���[�𐶐�����B
		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �r���[�|�[�g�̐ݒ�
	{
		// ��ʂ̂ǂ̗̈��DirectX�ŕ`�������\�����邩�̐ݒ�B
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screen_width);
		viewport.Height = static_cast<float>(screen_height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewport);
	}
	
	// �X�e�[�g�쐬
	{
		// �[�x�X�e���V���X�e�[�g�쐬
		CreateDeptthStencilState();

		// ���X�e���C�U�[�X�e�[�g�쐬
		CreateRasterizerState();

		// �u�����h�X�e�[�g�쐬
		CreateBlendState();
	}

	// �V�F�[�_�[
	{
		// �X�v���C�g�V�F�[�_�[
		sprite_shader = std::make_unique<SpriteShader>(device.Get());
		skybox_shader = std::make_unique<SkyBoxShader>(device.Get());
	}
	// �����_��
	{
		debug_renderer = std::make_unique<DebugRenderer>(device.Get());
		imgui_renderer = std::make_unique<ImGuiRenderer>(hwnd, device.Get());
	}

	// �X�N���[���e�N�X�`���쐬
	{
		scene_texture = std::make_unique<Texture>();
		scene_texture->Create(screen_width, screen_height, DXGI_FORMAT_R8G8B8A8_UNORM);

	}
	BGM = Audio::Instance().LoadAudioSource("Data\\Audio\\BGM\\BGM.wav", true);

}

//-----------------------------
// �f�X�g���N�^
//-----------------------------
Graphics::~Graphics()
{

}

//-----------------------------
// �r���[�|�[�g�ݒ�
//-----------------------------
void Graphics::SetViewport(float weidth, float height)
{
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = weidth;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);
}

//-----------------------------
// �����_�[�^�[�Q�b�g�ݒ�
//-----------------------------
void Graphics::SetRenderTargetView(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view)
{
	// �����_�[�^�[�Q�b�g�ݒ�
	context->OMSetRenderTargets(1, render_target_view, depth_stensil_view);
}

//-----------------------------
// ��ʃN���A
//-----------------------------
void Graphics::ScreenClear(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view, DirectX::XMFLOAT4 clear_color)
{
	// ��ʃN���A
	float color[4] = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
	context->ClearRenderTargetView(render_target_view[0], color);
	context->ClearDepthStencilView(depth_stensil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//-----------------------------
// �[�x�X�e���V���X�e�[�g�쐬
//-----------------------------
bool Graphics::CreateDeptthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = false;
	hr = device->CreateDepthStencilState(&desc, depth_stencil_state[(int)DepthStencilState::False].GetAddressOf());
	
	if (FAILED(hr))
	{
		assert("�f�v�X�X�e���V���X�e�[�g���s");
		return false;
	}

	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&desc, depth_stencil_state[(int)DepthStencilState::True].GetAddressOf());

	if (FAILED(hr))
	{
		assert("�f�v�X�X�e���V���X�e�[�g���s");
		return false;
	}

	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&desc, depth_stencil_state[(int)DepthStencilState::Write_False].GetAddressOf());

	if (FAILED(hr))
	{
		assert("�f�v�X�X�e���V���X�e�[�g���s");
		return false;
	}
	return true;
}

//-----------------------------
// ���X�e���C�U�[�X�e�[�g�쐬
//-----------------------------
bool Graphics::CreateRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC desc;

	for (int state = 0; state < Rasterize_Type; state++) {
		switch (state) {
		case (int)RasterizerState::Cull_Back:
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;

			break;

		case (int)RasterizerState::Wire:
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_WIREFRAME;
			desc.CullMode = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;
			break;

		case (int)RasterizerState::Cull_Front:
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_FRONT;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;

			break;

		case (int)RasterizerState::Cull_None:
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_NONE;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = true;
			desc.ScissorEnable = false;
			desc.MultisampleEnable = false;
			desc.AntialiasedLineEnable = false;

			break;
		}
		hr = device->CreateRasterizerState(&desc, rasterizer_state[state].GetAddressOf());

		if (FAILED(hr))
		{
			assert("���X�^���C�U�X�e�[�g�̐���");
			return false;
		}
	}

	return true;
}

//-----------------------------
// �u�����h�X�e�[�g�쐬
//-----------------------------
bool Graphics::CreateBlendState()
{
	HRESULT hr;
	D3D11_BLEND_DESC desc;

	for (int state = 0; state < Blend_Type; state++) {
		switch (state) {
		case (int)BlendState::None:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = false;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		case (int)BlendState::Alpha:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		case (int)BlendState::Add:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		case (int)BlendState::Subtract:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		case (int)BlendState::Replace:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;
		case (int)BlendState::Multiply:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;
		case (int)BlendState::Lighten:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		case (int)BlendState::Darken:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;
		case (int)BlendState::Screen:
			ZeroMemory(&desc, sizeof(desc));
			desc.IndependentBlendEnable = false;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			break;

		}
		//�u�����h�X�e�[�g�̍쐬
		hr = device->CreateBlendState(&desc, blend_state[state].GetAddressOf());

		if (FAILED(hr))
		{
			assert("�u�����h�X�e�[�g�̍쐬���s");
			return false;
		}
	}
	return true;
}
