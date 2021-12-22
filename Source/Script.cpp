#include "Script.h"
#include <memory>

//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
Script::Script(const char* filename)
{
	LoadScript(filename);
}

//----------------------------------------
// �f�X�g���N�^
//----------------------------------------
Script::~Script()
{
	delete[] text_buffer;
}

//----------------------------------------
// �X�N���v�g�ǂݍ��ݏ���
//----------------------------------------
bool Script::LoadScript(const char* filename)
{
	if (text_buffer)
		delete[] text_buffer;

	//�@�t�@�C����ǂݍ���
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "r");

	if (!fp) return false;

	// �t�@�C���T�C�Y���擾
	fseek(fp, 0, SEEK_END);
	text_buffer_size = ftell(fp);

	// �t�@�C����ǂݍ���
	text_buffer = new char[text_buffer_size];
	fseek(fp, 0, SEEK_SET);
	fread(text_buffer, text_buffer_size, 1, fp);

	// �t�@�C�������
	//fclose(fp);

	// �e�L�X�g�ʒu������������
	text_ip = 0;

	return true;
}

//----------------------------------------
// �e�L�X�g�t�@�C������Ӗ����镶����܂�
// �t�@�C���ʒu��i�߂鏈��
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
// ������p�����[�^�擾
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
// �����^�p�����[�^�擾
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
// �����^�p�����[�^�擾
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
// ���̃p�����[�^�ֈڂ�
//----------------------------------------
bool Script::SearchTop()
{
	for (; text_ip < text_buffer_size; ++text_ip)
	{
		// �R�����g���΂�
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

// �R���X�g���N�^
WriteScript::WriteScript()
{
}

// �f�X�g���N�^
WriteScript::~WriteScript()
{
}

// battle�V�[���ɑ���X�N���v�g��������
bool WriteScript::WriteSceneDataScript(const char* filename, const BattleSceneDataHeadder& data_headder)
{
	// �V�[���f�[�^�X�N���v�g�������ݏ���
	FILE* fp;
	fopen_s(&fp, filename, "w");

	//�@�t�@�C���I�[�v���Ɏ��s������false��Ԃ�
	if (!fp)	return false;

	fprintf(fp, "// �G�f�[�^ID\n");

	int data_size = static_cast<int>(data_headder.search_enemy_id.size());
	for (int i = 0; i < data_size; ++i)
	{
		fprintf(fp, "EnemyID %d\n", data_headder.search_enemy_id.at(i));
	}

	fprintf(fp, "END\n");

	// �t�@�C�������
	fclose(fp);

	return true;
}
