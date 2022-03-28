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

	// ������̏c�������Z�o
	DirectX::XMFLOAT2 CalcStringSize(const wchar_t* string, float xscale = 1.0f, float yscale = 1.0f);
	DirectX::XMFLOAT2 CalcStringSize(const std::string& string, float xscale = 1.0f, float yscale = 1.0f)
	{
		return	CalcStringSize(StringToWString(string).c_str(), xscale, yscale);
	}

	// string��wstring�֕ϊ�����
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
		static const WORD			NonCode = 0;			// �R�[�h�Ȃ�
		static const WORD			EndCode = 0xFFFF;		// �I���R�[�h
		static const WORD			ReturnCode = 0xFFFE;	// ���s�R�[�h
		static const WORD			TabCode = 0xFFFD;		// �^�u�R�[�h
		static const WORD			SpaceCode = 0xFFFC;		// �󔒃R�[�h

		float						left;					// �e�N�X�`�������W
		float						top;					// �e�N�X�`������W
		float						right;					// �e�N�X�`���E���W
		float						bottom;					// �e�N�X�`�������W
		float						xoffset;				// �I�t�Z�b�gX
		float						yoffset;				// �I�t�Z�b�gY
		float						xadvance;				// �v���|�[�V���i����
		float						width;					// �摜��
		float						height;					// �摜����
		int							page;					// �e�N�X�`���ԍ�
		bool						ascii;					// �A�X�L�[����
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
