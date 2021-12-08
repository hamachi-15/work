#pragma once
#include <Windows.h>

class Script
{
public:
	// �R���X�g���N�^
	Script(const char* filename);

	// �f�X�g���N�^
	~Script();

	// �X�N���v�g�ǂݍ��ݏ���
	bool LoadScript(const char* filename);

	// �e�L�X�g�t�@�C������Ӗ����镶����܂Ńt�@�C���ʒu��i�߂鏈��
	bool SearchParamTop();

	// ������p�����[�^�擾
	void GetParamString(LPSTR buf);

	// �����^�p�����[�^�擾
	int GetParamInt();

	// ���������_�^�̐��l�p�����[�^�擾
	float GetParamFloat();

	// ���̃p�����[�^�ֈڂ�
	bool SearchTop();
private:
	char* text_buffer = nullptr;
	int text_buffer_size;
	int text_ip;
};