#include "SpriteLayout.h"

int SpriteLayout::ShiftJisToUnicode(const char* shift_jis, wchar_t* unicode, uint64_t unicode_buffer_size)
{
	const size_t str_len = strlen(shift_jis) + 1;
	return MultiByteToWideChar(CP_ACP, 0, shift_jis, static_cast<int>(str_len) + 1, unicode, static_cast<int>(unicode_buffer_size));
}

int SpriteLayout::UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8_buffer_size)
{
	const size_t str_len = wcslen(unicode) + 1;
	return	WideCharToMultiByte(CP_UTF8, 0, unicode, static_cast<int>(str_len) + 1, utf8, static_cast<int>(utf8_buffer_size), NULL, NULL);
}

int SpriteLayout::ShiftJisToUTF8(const char* shift_jis, char* utf8, uint64_t unicode_buffer_size)
{
	wchar_t	str_unicode[MAX_PATH];
	if (ShiftJisToUnicode(shift_jis, str_unicode, MAX_PATH) > 0)
	{
		return	UnicodeToUTF8(str_unicode, utf8, unicode_buffer_size);
	}
	return	0;
}

int SpriteLayout::UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicode_buffer_size)
{
	const	SIZE_T	strLen = strlen(utf8) + 1;
	return	MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(strLen) + 1, unicode, static_cast<int>(unicode_buffer_size));
}

int SpriteLayout::UnicodeToShiftJis(const wchar_t* unicode, char* shift_jis, uint64_t shift_jis_buffer_size)
{
	const	SIZE_T	strLen = wcslen(unicode) + 1;
	return	WideCharToMultiByte(CP_ACP, 0, unicode, static_cast<int>(strLen) + 1, shift_jis, static_cast<int>(shift_jis_buffer_size), NULL, NULL);
}

int SpriteLayout::UTF8ToShiftJis(const char* utf8, char* shift_jis, uint64_t shift_jis_buffer_size)
{
	wchar_t	strUnicode[MAX_PATH];
	if (UTF8ToUnicode(utf8, strUnicode, MAX_PATH) > 0)
	{
		return	UnicodeToShiftJis(strUnicode, shift_jis, shift_jis_buffer_size);
	}
	return	0;
}

void SpriteLayout::Draw(ID3D11DeviceContext* context, DisplayInfo info, DirectX::XMFLOAT4 color)
{
	float texture_width = static_cast<float>(info.sprite->GetTextureWidth());
	float texture_height = static_cast<float>(info.sprite->GetTextureHeight());
	float width = texture_width * info.uv_split_size * info.corner_size_rate.x;
	float height = texture_height * info.uv_split_size * info.corner_size_rate.y;
	if (info.size.x < width * 2)
		width = info.size.x / 2;
	if (info.size.y < height * 2)
		height = info.size.y / 2;
	// 座標・UV値算出
	const DirectX::XMFLOAT2 positions[4][4] = {
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y - info.size.y + 0.0f}
		},
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y - info.size.y + height * 1.0f}
		},

		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y + info.size.y - height * 1.0f}
		},

		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y + info.size.y + 0.0f}
		}
	};
	const	DirectX::XMFLOAT2	uvs[4][4] =
	{
		{
			{                     0.0f,                    0.0f},
			{0.0f + info.uv_split_size,                    0.0f},
			{1.0f - info.uv_split_size,                    0.0f},
			{                     1.0f,                    0.0f}
		},
		{
			{                     0.0f, 0.0f + info.uv_split_size},
			{0.0f + info.uv_split_size, 0.0f + info.uv_split_size},
			{1.0f - info.uv_split_size, 0.0f + info.uv_split_size},
			{                     1.0f, 0.0f + info.uv_split_size}
		},
		{
			{                     0.0f,	1.0f - info.uv_split_size},
			{0.0f + info.uv_split_size, 1.0f - info.uv_split_size},
			{1.0f - info.uv_split_size, 1.0f - info.uv_split_size},
			{					  1.0f,	1.0f - info.uv_split_size}
		},
		{
			{                     0.0f,                    1.0f},
			{0.0f + info.uv_split_size,                    1.0f},
			{1.0f - info.uv_split_size,                    1.0f},
			{                     1.0f,					   1.0f}
		}
	};
	//	ウィンドウ描画
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			info.sprite->Render(context,
				info.center.x + (positions[y + 0][x + 0].x - info.center.x),
				info.center.y + (positions[y + 0][x + 0].y - info.center.y),
				(positions[y + 1][x + 1].x - positions[y + 0][x + 0].x),
				(positions[y + 1][x + 1].y - positions[y + 0][x + 0].y),

				texture_width * uvs[y + 0][x + 0].x,
				texture_height * uvs[y + 0][x + 0].y,
				texture_width * (uvs[y + 1][x + 1].x - uvs[y + 0][x + 0].x),
				texture_height * (uvs[y + 1][x + 1].y - uvs[y + 0][x + 0].y),
				0, Sprite::Pivot::CenterCenter,
				color.x, color.y, color.z, color.w);
		}
	}
}

DirectX::XMFLOAT4 SpriteLayout::CalcDisplayRect(DisplayInfo info)
{
	float texture_width = static_cast<float>(info.sprite->GetTextureWidth());
	float texture_height = static_cast<float>(info.sprite->GetTextureHeight());
	float width = texture_width * info.uv_split_size * info.corner_size_rate.x;
	float height = texture_height * info.uv_split_size * info.corner_size_rate.y;
	if (info.size.x < width * 2)
		width = info.size.x / 2;
	if (info.size.y < height * 2)
		height = info.size.y / 2;
	float	l = info.center.x - info.size.x + width  * 1.0f;
	float	t = info.center.y - info.size.y + height * 1.0f;
	float	r = info.center.x + info.size.x - width  * 1.0f;
	float	b = info.center.y + info.size.y - height * 1.0f;

	return	{ l, t, r - l, b - t };
}
