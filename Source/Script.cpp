#include "Script.h"
#include <memory>

//----------------------------------------
// コンストラクタ
//----------------------------------------
Script::Script(const char* filename)
{
	LoadScript(filename);
}

//----------------------------------------
// デストラクタ
//----------------------------------------
Script::~Script()
{
	delete[] text_buffer;
}

//----------------------------------------
// スクリプト読み込み処理
//----------------------------------------
bool Script::LoadScript(const char* filename)
{
	if (text_buffer)
		delete[] text_buffer;

	//　ファイルを読み込む
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "r");

	if (!fp) return false;

	// ファイルサイズを取得
	fseek(fp, 0, SEEK_END);
	text_buffer_size = ftell(fp);

	// ファイルを読み込む
	text_buffer = new char[text_buffer_size];
	fseek(fp, 0, SEEK_SET);
	fread(text_buffer, text_buffer_size, 1, fp);

	// ファイルを閉じる
	//fclose(fp);

	// テキスト位置を初期化する
	text_ip = 0;

	return true;
}

//----------------------------------------
// テキストファイルから意味ある文字列まで
// ファイル位置を進める処理
//----------------------------------------
bool Script::SearchParamTop()
{
	for (; text_ip < text_buffer_size; text_ip++)
	{
		if (text_buffer[text_ip] == '\t') continue;
		if (text_buffer[text_ip] == ' ') continue;
		if (text_buffer[text_ip] == ',') continue;
		if (text_buffer[text_ip] == '\n') {
			text_ip++;
		}
		return true;
	}
	return false;
}

//----------------------------------------
// 文字列パラメータ取得
//----------------------------------------
void Script::GetParamString(LPSTR buf)
{
	char temp, n;
	SearchParamTop();

	for (n = 0; ; ++text_ip, ++n)
	{
		temp = text_buffer[text_ip];
		if(temp == ',') break;
		if(temp == ' ') break;
		if(temp == '\t') break;
		if (temp == '}') { text_ip--; break; }
		if (temp == 0x0d || temp == 0x0a)
		{
			if (text_buffer[text_ip + 1] == 0x0d || text_buffer[text_ip + 1] == 0x0a)
			{
				text_ip++;
			}
			break;
		}
		buf[n] = temp;
		if (IsDBCSLeadByte(temp))
		{
			buf[n + 1] = text_buffer[text_ip + 1];
			n++;
			text_ip++;
		}
	}
	text_ip++;
	buf[n] = '\0';

}

//----------------------------------------
// 整数型パラメータ取得
//----------------------------------------
int Script::GetParamInt()
{
	int num;
	char temp_string[16];

	SearchParamTop();
	GetParamString(temp_string);
	num = atoi(temp_string);

	return num;
}

//----------------------------------------
// 整数型パラメータ取得
//----------------------------------------
float Script::GetParamFloat()
{
	float num;
	char temp_string[16];

	SearchParamTop();
	GetParamString(temp_string);
	num = static_cast<float>(atoi(temp_string));

	return num;
}

//----------------------------------------
// 次のパラメータへ移る
//----------------------------------------
bool Script::SearchTop()
{
	for (; text_ip < text_buffer_size; ++text_ip)
	{
		// コメントを飛ばす
		if ((text_buffer[text_ip] == '/') &&
			(text_buffer[text_ip + 1] == '/'))
		{
			for (; text_buffer[text_ip] != '\n'; ++text_ip)
			{
			}
			continue;
		}
		if(text_buffer[text_ip] == ' ') continue;
		if(text_buffer[text_ip] == '\t') continue;
		if(text_buffer[text_ip] == '\n') continue;
		if(text_buffer[text_ip] == 0x0d) continue;
		if(text_buffer[text_ip] == '}') continue;

		return true;
	}

	return false;
}

// コンストラクタ
WriteScript::WriteScript()
{
}

// デストラクタ
WriteScript::~WriteScript()
{
}

// battleシーンに送るスクリプト書き込み
bool WriteScript::WriteSceneDataScript(const char* filename, const BattleSceneDataHeadder& data_headder)
{
	// シーンデータスクリプト書き込み処理
	FILE* fp;
	fopen_s(&fp, filename, "w");

	//　ファイルオープンに失敗したらfalseを返す
	if (!fp)	return false;

	fprintf(fp, "// 敵データID\n");

	int data_size = static_cast<int>(data_headder.search_enemy_id.size());
	for (int i = 0; i < data_size; ++i)
	{
		fprintf(fp, "EnemyID %d\n", data_headder.search_enemy_id.at(i));
	}

	fprintf(fp, "END\n");

	// ファイルを閉じる
	fclose(fp);

	return true;
}
