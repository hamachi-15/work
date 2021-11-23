#pragma once

class Scene
{
public:
	// �R���X�g���N�^
	Scene(){}
	// �f�X�g���N�^
	virtual ~Scene(){}

	// ����������
	virtual void Initialize() = 0;
	
	// �I��������
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update(float elapsed_time) = 0;
	
	// �`�揈��
	virtual void Render() = 0;

	// ����������
	bool IsReady() const { return ready; };
	void SetReady() { ready = true; }
private:
	bool ready = false;
};