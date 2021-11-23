#include <memory>
#include <wrl.h>
#include "Shader.h"
#include "Graphics.h"

D3D11_INPUT_ELEMENT_DESC texture_input_element_desc[] = {

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
D3D11_INPUT_ELEMENT_DESC model_input_element_desc[] = {

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Shader::CSOBuffer Shader::GetCsoData(const char* filename)
{
	CSOBuffer buffer;

	// ファイルを開く
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "rb");
	_ASSERT_EXPR_A(fp, "CSO File not found");

	// ファイルのサイズを求める
	fseek(fp, 0, SEEK_END);
	buffer.size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// メモリ上に頂点シェーダーデータを格納する領域を用意する
	buffer.data = std::make_unique<u_char[]>(buffer.size);
	fread(buffer.data.get(), buffer.size, 1, fp);
	fclose(fp);

	return buffer;
}


void Shader::Activate(ID3D11DeviceContext* context)
{	
	// 入力レイアウト設定
	context->IASetInputLayout(vertex_layout.Get());
	context->VSSetShader(vertex_shader.Get(), NULL, 0);
	context->GSSetShader(geometry_shader.Get(), NULL, 0);
	context->PSSetShader(pixel_shader.Get(), NULL, 0);
}

void Shader::InActivate(ID3D11DeviceContext* context)
{
	context->VSSetShader(NULL, NULL, 0);
	context->GSSetShader(NULL, NULL, 0);
	context->PSSetShader(NULL, NULL, 0);
}

bool Shader::Create(ID3D11Device* device, const char* vsfilename, const char* psfilename, bool texflg)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();
	UINT num_elements;
	// 2Dのシェーダーかどうか
	if (texflg)
	{
		num_elements = ARRAYSIZE(texture_input_element_desc);
		create_vs_from_cso(device, vsfilename, vertex_shader.GetAddressOf(), vertex_layout.GetAddressOf(), texture_input_element_desc, num_elements);
		create_ps_from_cso(device, psfilename, pixel_shader.GetAddressOf());
	}
	else
	{
		num_elements = ARRAYSIZE(model_input_element_desc);
		create_vs_from_cso(device, vsfilename, vertex_shader.GetAddressOf(), vertex_layout.GetAddressOf(), model_input_element_desc, num_elements);
		create_ps_from_cso(device, psfilename, pixel_shader.GetAddressOf());
	}

	// 入力レイアウト設定
	//context->IASetInputLayout(vertex_layout.Get());

	return false;
}

bool Shader::Create(ID3D11Device* device, const char* vsfilename, const char* psfilename)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* context = graphics.GetDeviceContext();

	D3D11_INPUT_ELEMENT_DESC texture_input_element_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT num_elements = ARRAYSIZE(texture_input_element_desc);
	create_vs_from_cso(device, vsfilename, vertex_shader.GetAddressOf(), vertex_layout.GetAddressOf(), texture_input_element_desc, num_elements);
	create_ps_from_cso(device, psfilename, pixel_shader.GetAddressOf());
	// 入力レイアウト設定
	context->IASetInputLayout(vertex_layout.Get());

	return false;
}

bool Shader::Create(ID3D11Device* device, const char* vsfilename, const char* gsfilename, const char* psfilename, bool texflg)
{
	UINT num_elements;
	// ２Dのシェーダーかどうか
	if (texflg)
	{
		num_elements = ARRAYSIZE(texture_input_element_desc);
		create_vs_from_cso(device, vsfilename, vertex_shader.GetAddressOf(), vertex_layout.GetAddressOf(), texture_input_element_desc, num_elements);
		create_ps_from_cso(device, psfilename, pixel_shader.GetAddressOf());
		create_gs_from_cso(device, gsfilename, geometry_shader.GetAddressOf());
	}
	else
	{
		num_elements = ARRAYSIZE(model_input_element_desc);
		create_vs_from_cso(device, vsfilename, vertex_shader.GetAddressOf(), vertex_layout.GetAddressOf(), model_input_element_desc, num_elements);
		create_ps_from_cso(device, psfilename, pixel_shader.GetAddressOf());
		create_gs_from_cso(device, gsfilename, geometry_shader.GetAddressOf());
	}
	

	// 入力レイアウト設定
	//context->IASetInputLayout(vertex_layout.Get());

	return false;
}

HRESULT Shader::create_vs_from_cso(
	ID3D11Device* device, 
	const char* cso_name, 
	ID3D11VertexShader** vertex_shader, 
	ID3D11InputLayout** input_layout, 
	D3D11_INPUT_ELEMENT_DESC* input_element_desc, 
	UINT num_elements)
{
	CSOBuffer buffer;
	HRESULT hr = S_OK;
	// csoデータ取得
	buffer = GetCsoData(cso_name);

	// 頂点シェーダ生成
	hr = device->CreateVertexShader(buffer.data.get(), buffer.size, nullptr, vertex_shader);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 入力レイアウト生成
	hr = device->CreateInputLayout(input_element_desc, num_elements, buffer.data.get(), buffer.size, input_layout);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	
	return hr;
}

HRESULT Shader::create_ps_from_cso(
	ID3D11Device* device, 
	const char* cso_name, 
	ID3D11PixelShader** pixel_shader)
{
	CSOBuffer buffer;
	HRESULT hr = S_OK;
	// csoデータ取得
	buffer = GetCsoData(cso_name);

	// ピクセルシェーダー生成
	hr = device->CreatePixelShader(buffer.data.get(), buffer.size, nullptr, pixel_shader);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	return hr;
}

HRESULT Shader::create_gs_from_cso(
	ID3D11Device* device,
	const char* cso_name, 
	ID3D11GeometryShader** geometry_shader)
{
	CSOBuffer buffer;
	HRESULT hr = S_OK;
	// csoデータ取得
	buffer = GetCsoData(cso_name);

	// ジオメトリシェーダー生成
	hr = device->CreateGeometryShader(buffer.data.get(), buffer.size,  nullptr, geometry_shader);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	
	return hr;
}

