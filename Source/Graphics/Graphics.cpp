#include "Misc.h"
#include "Graphics.h"
#include "Texture.h"

#include "ShaderManager.h"
#include "EffectManager.h"
#include "AudioManager.h"

#include "SpriteShader.h"
#include "SkyBoxShader.h"
#include "2DPrimitive.h"
#include "GaussianBlurShader.h"
#include "BloomShader.h"
#include "CreateShadowMapShader.h"
#include "CascadeShadowMapShader.h"
#include "PhongShader.h"
#include "LambertShader.h"
#include "DissolveShader.h"

Graphics* Graphics::instance = nullptr;


//-----------------------------
// コンストラクタ
//-----------------------------
Graphics::Graphics(HWND hwnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	// 画面のサイズを取得する。
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT screen_width = rc.right - rc.left;
	UINT screen_height = rc.bottom - rc.top;

	this->screen_width = static_cast<float>(screen_width);
	this->screen_height = static_cast<float>(screen_height);

	HRESULT hr = S_OK;

	// デバイス＆スワップチェーンの生成
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

		// スワップチェーンを作成するための設定オプション
		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		{
			swapchain_desc.BufferDesc.Width = screen_width;
			swapchain_desc.BufferDesc.Height = screen_height;
			swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 1ピクセルあたりの各色(RGBA)を8bit(0〜255)のテクスチャ(バックバッファ)を作成する。
			swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = 1;		// バックバッファの数
			swapchain_desc.OutputWindow = hwnd;	// DirectXで描いた画を表示するウインドウ
			swapchain_desc.Windowed = FALSE;		// ウインドウモードか、フルスクリーンにするか。
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchain_desc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		}	

		D3D_FEATURE_LEVEL feature_level;
		// デバイス＆スワップチェーンの生成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,						// どのビデオアダプタを使用するか？既定ならばnullptrで、IDXGIAdapterのアドレスを渡す。
			D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。D3D_DRIVER_TYPE_HARDWARE 以外は基本的にソフトウェア実装で、特別なことをする場合に用いる。
			nullptr,						// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定した際に、その処理を行うDLLのハンドルを渡す。それ以外を指定している際には必ずnullptrを渡す。
			create_device_flags,			// 何らかのフラグを指定する。詳しくはD3D11_CREATE_DEVICE列挙型で検索。
			feature_levels,					// D3D_FEATURE_LEVEL列挙型の配列を与える。nullptrにすることでも上記featureと同等の内容の配列が使用される。
			ARRAYSIZE(feature_levels),		// featureLevels配列の要素数を渡す。
			D3D11_SDK_VERSION,				// SDKのバージョン。必ずこの値。
			&swapchain_desc,				// ここで設定した構造体に設定されているパラメータでSwapChainが作成される。
			swapchain.GetAddressOf(),		// 作成が成功した場合に、SwapChainのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でSwapChainを操作する。
			device.GetAddressOf(),			// 作成が成功した場合に、Deviceのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でDeviceを操作する。
			&feature_level,					// 作成に成功したD3D_FEATURE_LEVELを格納するためのD3D_FEATURE_LEVEL列挙型変数のアドレスを設定する。
			context.GetAddressOf()			// 作成が成功した場合に、Contextのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でContextを操作する。
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}


	// 深度ステンシルビューの生成
	{
		// 深度ステンシル情報を書き込むためのテクスチャを作成する。
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		depthStencilBufferDesc.Width = screen_width;
		depthStencilBufferDesc.Height = screen_height;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 1ピクセルあたり、深度情報を24Bit / ステンシル情報を8bitのテクスチャを作成する。
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// 深度ステンシル用のテクスチャを作成する。
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depthStencilBufferDesc, nullptr, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


		// 深度ステンシルテクスチャへの書き込みに窓口になる深度ステンシルビューを作成する。
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// レンダーターゲットビューの生成
	{
		// スワップチェーンからバックバッファテクスチャを取得する。
		// ※スワップチェーンに内包されているバックバッファテクスチャは'色'を書き込むテクスチャ。
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// バックバッファテクスチャへの書き込みの窓口となるレンダーターゲットビューを生成する。
		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ビューポートの設定
	{
		// 画面のどの領域にDirectXで描いた画を表示するかの設定。
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screen_width);
		viewport.Height = static_cast<float>(screen_height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewport);
	}
	
	// ステート作成
	{
		// 深度ステンシルステート作成
		CreateDeptthStencilState();

		// ラステライザーステート作成
		CreateRasterizerState();

		// ブレンドステート作成
		CreateBlendState();
	}

	// シェーダー
	{
		ShaderManager& shader_manager = ShaderManager::Instance();
		// スプライトシェーダー
		shader_manager.RegisterShader<SpriteShader>(device.Get());
		shader_manager.RegisterShader<Primitive>(device.Get());
		shader_manager.RegisterShader<SkyBoxShader>(device.Get());
		//shader_manager.RegisterShader<GaussianBlur>(device.Get());
		shader_manager.RegisterShader<Bloom>(device.Get());
		shader_manager.RegisterShader<DissolveShader>(device.Get());
		shader_manager.RegisterShader<Phong>(device.Get());
		shader_manager.RegisterShader<LambertShader>(device.Get());
		shader_manager.RegisterShader<CreateShadowMap>(device.Get());
		shader_manager.RegisterShader<CascadeShadowMap>(device.Get());
	}
	// レンダラ
	{
		debug_renderer = std::make_unique<DebugRenderer>(device.Get());
		imgui_renderer = std::make_unique<ImGuiRenderer>(hwnd, device.Get());
	}

	// スクリーンテクスチャ作成
	{
		scene_texture = std::make_unique<Texture>();
		scene_texture->Create(screen_width, screen_height, DXGI_FORMAT_R8G8B8A8_UNORM);

	}

	// サウンド
	{
		// SE登録
		AudioManager::Instance().RegisterSoundEffect("Data/Audio/SE/SwordHit.wav", false);
		AudioManager::Instance().RegisterSoundEffect("Data/Audio/SE/PlayerGetHit.wav", false);
		AudioManager::Instance().RegisterSoundEffect("Data/Audio/SE/PlayerWalk.wav", false);
		AudioManager::Instance().RegisterSoundEffect("Data/Audio/SE/DragonVoice.wav", false);
		// BGM登録
		AudioManager::Instance().RegisterBGM("Data/Audio/BGM/TitleBGM.wav", true);
		AudioManager::Instance().RegisterBGM("Data/Audio/BGM/WorldMapBGM.wav", true);
		AudioManager::Instance().RegisterBGM("Data/Audio/BGM/ButtleBGM.wav", true);
		AudioManager::Instance().RegisterBGM("Data/Audio/BGM/GameClearBGM.wav", true);
		AudioManager::Instance().RegisterBGM("Data/Audio/BGM/GameOverBGM.wav", true);
	}

	// エフェクトマネージャー初期化
	{
		effect_manager = std::make_shared<EffectManager>();
		effect_manager->Initialize();
	}
}

//-----------------------------
// デストラクタ
//-----------------------------
Graphics::~Graphics()
{

}

//-----------------------------
// ビューポート設定
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
// レンダーターゲット設定
//-----------------------------
void Graphics::SetRenderTargetView(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view)
{
	// レンダーターゲット設定
	context->OMSetRenderTargets(1, render_target_view, depth_stensil_view);

}

//-----------------------------
// 画面クリア
//-----------------------------
void Graphics::ScreenClear(ID3D11RenderTargetView* render_target_view[], ID3D11DepthStencilView* depth_stensil_view, DirectX::XMFLOAT4 clear_color)
{
	// 画面クリア
	float color[4] = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
	context->ClearRenderTargetView(render_target_view[0], color);
	context->ClearDepthStencilView(depth_stensil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//-----------------------------
// 深度ステンシルステート作成
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
		assert("デプスステンシルステート失敗");
		return false;
	}

	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&desc, depth_stencil_state[(int)DepthStencilState::True].GetAddressOf());

	if (FAILED(hr))
	{
		assert("デプスステンシルステート失敗");
		return false;
	}

	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = device->CreateDepthStencilState(&desc, depth_stencil_state[(int)DepthStencilState::Write_False].GetAddressOf());

	if (FAILED(hr))
	{
		assert("デプスステンシルステート失敗");
		return false;
	}
	return true;
}

//-----------------------------
// ラステライザーステート作成
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
			assert("ラスタライザステートの生成");
			return false;
		}
	}

	return true;
}

//-----------------------------
// ブレンドステート作成
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
		//ブレンドステートの作成
		hr = device->CreateBlendState(&desc, blend_state[state].GetAddressOf());

		if (FAILED(hr))
		{
			assert("ブレンドステートの作成失敗");
			return false;
		}
	}
	return true;
}
