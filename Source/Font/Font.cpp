#include <WICTextureLoader.h>
#include "Logger.h"
#include "Misc.h"
#include "Font.h"
#include "Graphics.h"

Font::Font(const char* filename, int max_sprite_count)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	// シェーダー作成
	{
		shader = std::make_unique<FontShader>(device);
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * max_sprite_count * 4);
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// インデックスバッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		std::unique_ptr<UINT[]> indices = std::make_unique<UINT[]>((UINT)(max_sprite_count * 6));
		// 四角形 を 三角形 ２つに展開
		// 0---1      0---1  4
		// |   |  →  |／  ／|
		// 2---3      2  3---5
		{
			UINT* p = indices.get();
			for (int i = 0; i < max_sprite_count * 4; i += 4)
			{
				p[0] = i + 0;
				p[1] = i + 1;
				p[2] = i + 2;
				p[3] = i + 2;
				p[4] = i + 1;
				p[5] = i + 3;
				p += 6;
			}
		}

		desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * max_sprite_count * 6);
		//desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresource_data.pSysMem = indices.get();
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		HRESULT hr = device->CreateBuffer(&desc, &subresource_data, index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// .fnt 読み込み
	{
		FILE* fp = nullptr;
		fopen_s(&fp, filename, "rb");
		_ASSERT_EXPR_A(fp, "FNT File not found");

		fseek(fp, 0, SEEK_END);
		long fntSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::unique_ptr<char[]> fnt_data = std::make_unique<char[]>(fntSize);
		fread(fnt_data.get(), fntSize, 1, fp);
		fclose(fp);

		// (***.fnt)ヘッダ解析
		char* context = nullptr;	// 内部的に使用するので深く考えない
		const char* delimiter = " ,=\"\r\n";
		const char* delimiter2 = ",=\"\r\n";
		char* pToken = ::strtok_s(fnt_data.get(), delimiter, &context);
		_ASSERT_EXPR_A(::strcmp(pToken, "info") == 0, "FNT Format");

		// face
		::strtok_s(nullptr, delimiter, &context);
		const char* face = ::strtok_s(nullptr, delimiter2, &context);
		// size
		::strtok_s(nullptr, delimiter, &context);
		int size = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// bold
		::strtok_s(nullptr, delimiter, &context);
		int bold = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// italic
		::strtok_s(nullptr, delimiter, &context);
		int italic = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// charset
		::strtok_s(nullptr, delimiter, &context);
		const char* charset = ::strtok_s(nullptr, delimiter, &context);
		// unicode
		if (::strcmp(charset, "unicode") == 0)
			charset = "";
		else
			::strtok_s(nullptr, delimiter, &context);
		int unicode = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// stretchH
		::strtok_s(nullptr, delimiter, &context);
		int stretchH = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// smooth
		::strtok_s(nullptr, delimiter, &context);
		int smooth = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// aa
		::strtok_s(nullptr, delimiter, &context);
		int aa = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// padding
		::strtok_s(nullptr, delimiter, &context);
		int padding1 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding2 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding3 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding4 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// spacing
		::strtok_s(nullptr, delimiter, &context);
		int spacing1 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int spacing2 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// outline
		::strtok_s(nullptr, delimiter, &context);
		int outline = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// common
		::strtok_s(nullptr, delimiter, &context);
		// lineHeight
		::strtok_s(nullptr, delimiter, &context);
		int lineHeight = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// base
		::strtok_s(nullptr, delimiter, &context);
		int base = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// scaleW
		::strtok_s(nullptr, delimiter, &context);
		int scaleW = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// scaleH
		::strtok_s(nullptr, delimiter, &context);
		int scaleH = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// pages
		::strtok_s(nullptr, delimiter, &context);
		int pages = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// packed
		::strtok_s(nullptr, delimiter, &context);
		int packed = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// alphaChnl
		::strtok_s(nullptr, delimiter, &context);
		int alphaChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// redChnl
		::strtok_s(nullptr, delimiter, &context);
		int redChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// greenChnl
		::strtok_s(nullptr, delimiter, &context);
		int greenChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// blueChnl
		::strtok_s(nullptr, delimiter, &context);
		int blueChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// ディレクトリパス取得
		char dirname[256];
		::_splitpath_s(filename, nullptr, 0, dirname, 256, nullptr, 0, nullptr, 0);
		shader_resource_views.resize(pages);
		for (int i = 0; i < pages; i++)
		{
			// page
			::strtok_s(nullptr, delimiter, &context);
			// id
			::strtok_s(nullptr, delimiter, &context);
			int id = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// file
			::strtok_s(nullptr, delimiter, &context);
			const char* file = ::strtok_s(nullptr, delimiter2, &context);

			// 相対パスの解決
			char fname[256];
			::_makepath_s(fname, 256, nullptr, dirname, file, nullptr);

			// マルチバイト文字からワイド文字へ変換
			size_t length;
			wchar_t wfname[256];
			::mbstowcs_s(&length, wfname, 256, fname, _TRUNCATE);

			// テクスチャ読み込み
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfname, resource.GetAddressOf(), shader_resource_views.at(i).GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// chars
		::strtok_s(nullptr, delimiter, &context);
		// count
		::strtok_s(nullptr, delimiter, &context);
		int count = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// データ格納
		font_width = static_cast<float>(size);
		font_height = static_cast<float>(lineHeight);
		texture_count = pages;
		character_count = count + 1;
		character_infos.resize(character_count);
		character_indices.resize(0xFFFF);
		::memset(character_indices.data(), 0, sizeof(WORD) * character_indices.size());

		character_indices.at(0x00) = CharacterInfo::EndCode;
		character_indices.at(0x0a) = CharacterInfo::ReturnCode;
		character_indices.at(0x09) = CharacterInfo::TabCode;
		character_indices.at(0x20) = CharacterInfo::SpaceCode;

		// 文字情報解析
		int n = 1;
		for (int i = 0; i < count; i++)
		{
			// char
			::strtok_s(nullptr, delimiter, &context);
			// id
			::strtok_s(nullptr, delimiter, &context);
			int id = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// x
			::strtok_s(nullptr, delimiter, &context);
			int x = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// y
			::strtok_s(nullptr, delimiter, &context);
			int y = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// width
			::strtok_s(nullptr, delimiter, &context);
			int width = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// height
			::strtok_s(nullptr, delimiter, &context);
			int height = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// xoffset
			::strtok_s(nullptr, delimiter, &context);
			int xoffset = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// yoffset
			::strtok_s(nullptr, delimiter, &context);
			int yoffset = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// xadvance
			::strtok_s(nullptr, delimiter, &context);
			int xadvance = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// page
			::strtok_s(nullptr, delimiter, &context);
			int page = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// chnl
			::strtok_s(nullptr, delimiter, &context);
			int chnl = ::atoi(::strtok_s(nullptr, delimiter, &context));

			// 文字コード格納
			if (id >= 0x10000) continue;

			CharacterInfo& info = character_infos.at(n);

			character_indices.at(id) = static_cast<WORD>(i + 1);

			// 文字情報格納
			info.left = static_cast<float>(x) / static_cast<float>(scaleW);
			info.top = static_cast<float>(y) / static_cast<float>(scaleH);
			info.right = static_cast<float>(x + width) / static_cast<float>(scaleW);
			info.bottom = static_cast<float>(y + height) / static_cast<float>(scaleH);
			info.xoffset = static_cast<float>(xoffset);
			info.yoffset = static_cast<float>(yoffset);
			info.xadvance = static_cast<float>(xadvance);
			info.width = static_cast<float>(width);
			info.height = static_cast<float>(height);
			info.page = page;
			info.ascii = (id < 0x100);

			n++;
		}
	}

}

void Font::Begin(ID3D11DeviceContext* context)
{
	// スクリーンサイズ取得
	D3D11_VIEWPORT viewport;
	UINT num_viewports = 1;
	context->RSGetViewports(&num_viewports, &viewport);
	screen_width = viewport.Width;
	screen_height = viewport.Height;

	// 頂点編集開始
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

	current_vertex = reinterpret_cast<Vertex*>(mapped_subresource.pData);
	current_index_count = 0;
	current_page = -1;
	subsets.clear();
}

void Font::Draw(float x, float y, const wchar_t* string, float xscale, float yscale, DirectX::XMFLOAT4 color)
{
	size_t length = ::wcslen(string);

	float start_x = x;
	float start_y = y;
	float space = font_width * xscale;

	for (size_t i = 0; i < length; ++i)
	{
		// 文字値から文字情報が格納されているコードを取得
		WORD word = static_cast<WORD>(string[i]);
		WORD code = character_indices.at(word);

		// 特殊制御用コードの処理
		if (code == CharacterInfo::EndCode)
		{
			break;
		}
		else if (code == CharacterInfo::ReturnCode)
		{
			x = start_x;
			y += font_height * yscale;
			continue;
		}
		else if (code == CharacterInfo::TabCode)
		{
			x += space * 4;
			continue;
		}
		else if (code == CharacterInfo::SpaceCode)
		{
			x += space;
			continue;
		}

		// 文字情報を取得し、頂点データを編集
		const CharacterInfo& info = character_infos.at(code);
		float xoffset = info.xoffset * xscale;
		float yoffset = info.yoffset * yscale;
		float width = info.width * xscale;
		float height = info.height * yscale;
		float xadvance = info.xadvance * xscale;
		float positionX = x + xoffset;// + 0.5f;
		float positionY = y + yoffset;// + 0.5f;

		// 0---1
		// |   |
		// 2---3
		current_vertex[0].position.x = positionX;
		current_vertex[0].position.y = positionY;
		current_vertex[0].position.z = 0.0f;
		current_vertex[0].texcoord.x = info.left;
		current_vertex[0].texcoord.y = info.top;
		current_vertex[0].color = color;

		current_vertex[1].position.x = positionX + width;
		current_vertex[1].position.y = positionY;
		current_vertex[1].position.z = 0.0f;
		current_vertex[1].texcoord.x = info.right;
		current_vertex[1].texcoord.y = info.top;
		current_vertex[1].color = color;

		current_vertex[2].position.x = positionX;
		current_vertex[2].position.y = positionY + height;
		current_vertex[2].position.z = 0.0f;
		current_vertex[2].texcoord.x = info.left;
		current_vertex[2].texcoord.y = info.bottom;
		current_vertex[2].color = color;

		current_vertex[3].position.x = positionX + width;
		current_vertex[3].position.y = positionY + height;
		current_vertex[3].position.z = 0.0f;
		current_vertex[3].texcoord.x = info.right;
		current_vertex[3].texcoord.y = info.bottom;
		current_vertex[3].color = color;

		// NDC座標変換
		for (int j = 0; j < 4; ++j)
		{
			current_vertex[j].position.x = 2.0f * current_vertex[j].position.x / screen_width - 1.0f;
			current_vertex[j].position.y = 1.0f - 2.0f * current_vertex[j].position.y / screen_height;
		}
		current_vertex += 4;

		x += xadvance;

		// テクスチャが切り替わる度に描画する情報を設定
		if (current_page != info.page)
		{
			current_page = info.page;

			Subset subset;
			subset.shader_resource_view = shader_resource_views.at(info.page).Get();
			subset.start_index = current_index_count;
			subset.index_count = 0;
			subsets.emplace_back(subset);
		}
		current_index_count += 6;
	}
}

void Font::End(ID3D11DeviceContext* context)
{
	// 頂点編集終了
	context->Unmap(vertex_buffer.Get(), 0);

	// 描画なしなら飛ばす
	if (subsets.size() == 0)
		return;

	// サブセット調整
	size_t size = subsets.size();
	for (size_t i = 1; i < size; ++i)
	{
		Subset& prev = subsets.at(i - 1);
		Subset& next = subsets.at(i);
		prev.index_count = next.start_index - prev.start_index;
	}
	Subset& last = subsets.back();
	last.index_count = current_index_count - last.start_index;

	// シェーダー設定
	shader->Begin(context);

	// レンダーステート設定
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	context->RSSetState(rasterizer_state.Get());
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	// 描画
	for (size_t i = 0; i < size; ++i)
	{
		Subset& subset = subsets.at(i);

		context->PSSetShaderResources(0, 1, &subset.shader_resource_view);
		context->DrawIndexed(subset.index_count, subset.start_index, 0);
	}
	shader->End(context);
}

DirectX::XMFLOAT2 Font::CalcStringSize(const wchar_t* string, float xscale, float yscale)
{
	size_t length = ::wcslen(string);

	float x = 0, y = 0;
	float start_x = x, start_y = y;
	float width = x, height = font_height * yscale;
	float space = font_width * xscale;

	for (size_t i = 0; i < length; ++i)
	{
		// 文字値から文字情報が格納されているコードを取得
		WORD word = static_cast<WORD>(string[i]);
		WORD code = character_indices.at(word);

		// 特殊制御用コードの処理
		if (code == CharacterInfo::EndCode)
		{
			break;
		}
		else if (code == CharacterInfo::ReturnCode)
		{
			if (width <= x)
				width = x;
			x = start_x;
			height += font_height * yscale;
			continue;
		}
		else if (code == CharacterInfo::TabCode)
		{
			x += space * 4;
			continue;
		}
		else if (code == CharacterInfo::SpaceCode)
		{
			x += space;
			continue;
		}

		// 文字情報を取得し、頂点データを編集
		const CharacterInfo& info = character_infos.at(code);
		x += info.xadvance * xscale;
	}
	if (width <= x)
		width = x;

	return { width, height };
}

std::wstring Font::StringToWString(const std::string& string)
{
	// SJIS -> wstring
	int size = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[size];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, cpUCS2
		, size);

	// stringの生成
	std::wstring oret(cpUCS2, cpUCS2 + size - 1);

	// バッファの破棄
	delete[] cpUCS2;

	// 変換結果を返す
	return(oret);
}
