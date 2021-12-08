#pragma once
#include "Telegram.h"
#include <string>

class Scene
{
public:
	// �R���X�g���N�^
	Scene(){}
	// �f�X�g���N�^
	virtual ~Scene(){}

	// �V�[�����ݒ�
	void SetName(const char* name) { this->name = name; }

	// �V�[�����擾
	const char* GetName() const { return name.c_str(); }

	// ����������
	virtual void Initialize() = 0;
	
	// �I��������
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update(float elapsed_time) = 0;
	
	// �`�揈��
	virtual void Render() = 0;

	virtual bool OnMessages(const Telegram& telegram) { return true; }

	// ����������
	bool IsReady() const { return ready; }
	void SetReady() { ready = true; }
private:
	bool ready = false;
	std::string name; // �V�[���̖��O
};