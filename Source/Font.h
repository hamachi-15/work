#pragma once
#include <vector>
#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "FontShader.h"

class Font
{
public:
	Font(const char* filename, int max_sprite_count);
	~Font() {}

	void Begin(ID3D11DeviceContext* context);
	void Draw(float x, float y, const wchar_t* string, float xscale = 1.0f, float yscale = 1.0f, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void Draw(float x, float y, const std::string& string, float xscale = 1.0f, float yscale = 1.0f, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f })
	{
		Draw(x, y, StringToWString(string).c_str(), xscale, yscale, color);
	}
	void End(ID3D11DeviceContext* context);

	// 文字列の縦横幅を算出
	DirectX::XMFLOAT2 CalcStringSize(const wchar_t* string, float xscale = 1.0f, float yscale = 1.0f);
	DirectX::XMFLOAT2 CalcStringSize(const std::string& string, float xscale = 1.0f, float yscale = 1.0f)
	{
		return	CalcStringSize(StringToWString(string).c_str(), xscale, yscale);
	}

	// stringをwstringへ変換する
	std::wstring StringToWString(const std::string& string);

private:
	std::unique_ptr<FontShader>									shader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>						vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>						pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>						input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>							vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>							index_buffer;
	Microsoft::WRL::ComPtr<ID3D11BlendState>						blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>					rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>					depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>						sampler_state;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	shader_resource_views;

	struct Vertex
	{
		DirectX::XMFLOAT3			position;
		DirectX::XMFLOAT4			color;
		DirectX::XMFLOAT2			texcoord;
	};

	struct CharacterInfo
	{
		static const WORD			NonCode = 0;			// コードなし
		static const WORD			EndCode = 0xFFFF;		// 終了コード
		static const WORD			ReturnCode = 0xFFFE;	// 改行コード
		static const WORD			TabCode = 0xFFFD;		// タブコード
		static const WORD			SpaceCode = 0xFFFC;		// 空白コード

		float						left;					// テクスチャ左座標
		float						top;					// テクスチャ上座標
		float						right;					// テクスチャ右座標
		float						bottom;					// テクスチャ下座標
		float						xoffset;				// オフセットX
		float						yoffset;				// オフセットY
		float						xadvance;				// プロポーショナル幅
		float						width;					// 画像幅
		float						height;					// 画像高さ
		int							page;					// テクスチャ番号
		bool						ascii;					// アスキー文字
	};

	float							font_width;
	float							font_height;
	int								texture_count;
	int								character_count;
	std::vector<CharacterInfo>		character_infos;
	std::vector<WORD>				character_indices;

	struct Subset
	{
		ID3D11ShaderResourceView* shader_resource_view;
		UINT						start_index;
		UINT						index_count;
	};
	std::vector<Subset>				subsets;
	Vertex* current_vertex = nullptr;
	UINT							current_index_count;
	int								current_page;

	float							screen_width;
	float							screen_height;
};
