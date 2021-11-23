#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "RenderContext.h"
#include "Model.h"
#include "Misc.h"
#include "Texture.h"

// TODO シェーダーの構築見直しをする

class Shader
{
public:
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometry_shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		vertex_layout = nullptr;
public:
	Shader() {}
	virtual ~Shader() {}

	// 名前取得
	virtual const char* GetShaderName()const = 0;
	// 描画開始処理
	virtual void Begin(ID3D11DeviceContext* context) {}
	virtual void Begin(ID3D11DeviceContext* context, RenderContext& render_context) {}
	virtual void Begin(ID3D11DeviceContext* context, BlurRenderContext& bulr_render_context) {}
	virtual void Begin(ID3D11DeviceContext* context, BlurRenderContext& bulr_render_context, Texture* texture) {}
	virtual void Begin(ID3D11DeviceContext* context, RenderContext& render_context, CubeMapRenderContext& cubemap_render_context){}
	virtual void Begin(ID3D11DeviceContext* context, Texture* texture) {}
	// 描画処理
	virtual void Draw(ID3D11DeviceContext* context, const Model* model) {};
	
	// 描画終了処理
	virtual void End(ID3D11DeviceContext* context) = 0;

	void Activate(ID3D11DeviceContext* context);
	void InActivate(ID3D11DeviceContext* context);

	bool Create(ID3D11Device* device, const char* vsfilename, const char* psfilename);
	bool Create(ID3D11Device* device, const char* vsfilename, const char* psfilename, bool texflg);
	bool Create(ID3D11Device* device, const char* vsfilename, const char* gsfilename, const char* psfilename, bool texflg = false);
	// TODO使うことがあれば実装
	//bool Create(WCHAR* vsfilename, WCHAR* dsfilename, WCHAR* hsfilename, WCHAR* psfilename);
	//bool Create(WCHAR* vsfilename, WCHAR* dsfilename, WCHAR* hsfilename, WCHAR* gsfilename, WCHAR* psfilename);

private:
	struct CSOBuffer
	{
		std::unique_ptr<u_char[]> data;
		long size;
	};

	HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
	HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);
	HRESULT create_gs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
	//HRESULT create_hs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11HullShader** hull_shader);

	// CSOファイルデータ取得
	CSOBuffer GetCsoData(const char* filename);


};
