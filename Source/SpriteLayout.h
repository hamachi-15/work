#pragma once
#include	"Sprite.h"
#include	"Font.h"
#include	<vector>
#include	<string>

namespace SpriteLayout
{
	// 文字コードの変換処理系関数
	int	ShiftJisToUnicode(const char* shift_jis, wchar_t* unicode, uint64_t unicode_buffer_size);
	int	UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8_buffer_size);
	int	ShiftJisToUTF8(const char* shift_jis, char* utf8, uint64_t unicode_buffer_size);
	int	UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicode_buffer_size);
	int	UnicodeToShiftJis(const wchar_t* unicode, char* shift_jis, uint64_t shift_jis_buffer_size);
	int	UTF8ToShiftJis(const char* utf8, char* shift_jis, uint64_t shift_jis_buffer_size);
	
	// テクスチャの縦横を3分割して表示する
	// ┏━┳━┳━┓
	// ┃１┃２┃３┃
	// ┣━╋━╋━┫
	// ┃４┃５┃６┃
	// ┣━╋━╋━┫
	// ┃７┃８┃９┃
	// ┗━┻━┻━┛
	// 【１・３・７・９】のサイズをテクスチャで指定した一定の値に規定しつつ、
	// 【２・４・５・６・８】のサイズを伸縮させて表示させる
	struct	DisplayInfo
	{
		Sprite* sprite;
		DirectX::XMFLOAT2	center;
		DirectX::XMFLOAT2	size;
		DirectX::XMFLOAT2	corner_size_rate;
		float				uv_split_size;
	};
	// レイアウト調整して表示。↑のブロックの定義どおり
	void Draw(ID3D11DeviceContext* context, DisplayInfo info, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 });

	// レイアウト調整後の表示矩形を算出。↑の【５】の矩形を算出する
	DirectX::XMFLOAT4 CalcDisplayRect(DisplayInfo info);

}