#pragma once
#include <Windows.h>
#include "ScriptDataHeadder.h"

class Script
{
public:
	// コンストラクタ
	Script(const char* filename);

	// デストラクタ
	~Script();

	// スクリプト読み込み処理
	bool LoadScript(const char* filename);

	// テキストファイルから意味ある文字列までファイル位置を進める処理
	bool SearchParamTop();

	// 文字列パラメータ取得
	void GetParamString(LPSTR buf);

	// 整数型パラメータ取得
	int GetParamInt();

	// 浮動小数点型の数値パラメータ取得
	float GetParamFloat();

	// 次のパラメータへ移る
	bool SearchTop();
private:
	char* text_buffer = nullptr;
	int text_buffer_size = 0;
	int text_ip = 0;
};

class WriteScript
{
private:
	// コンストラクタ
	WriteScript();
	// デストラクタ
	~WriteScript();
public:
	// インスタンス取得
	static WriteScript& Instance()
	{
		static WriteScript instance;
		return instance;
	}

	// シーンデータスクリプト書き込み処理
	bool WriteSceneDataScript(const char* filename, const BattleSceneDataHeadder& dataheadder);

};