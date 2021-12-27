#pragma once
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Shader.h"
#include "Model.h"

class Component;
//class Model;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	Actor() {}
	virtual ~Actor() {}

	// �J�n����
	virtual void Start();

	// �X�V����
	virtual void Update(float elapsed_time);

	// �`�揈��
	virtual void Draw(ID3D11DeviceContext* context);

	// �s��X�V
	virtual void UpdateTransform();

	// IMGUI�\��
	virtual void OnGUI();

	// ���f���̃Z�b�g�A�b�v
	void SetUpModel(const char* filename);

	// ���f���̎擾
	Model* GetModel() const { return model.get(); }

	// �A�j���[�V�������̌��_�m�[�h����ݒ�
	void SetAnimationNodeName(const char* node_name) { animation_node_origin = node_name; }

	// �V�F�[�_�[�̐ݒ�
	void SetShader(std::shared_ptr<Shader> shader) { this->shader = shader; }

	// �V�F�[�_�[�̎擾
	Shader* GetShader() const { return shader.get(); }

	// �G�l�~�[�f�[�^ID�ݒ�
	void SetEnemyDataID(int id) { this->id = id; }

	// �G�l�~�[�f�[�^ID�擾
	int GetEnemyDataID() const { return id; }
	
	// ���O�̐ݒ�
	void SetName(const char* name) { this->name = name; }

	// ���O�̎擾
	const char* GetName() const { return name.c_str(); }
	std::string GetNameOfString() const { return name; }

	// �ʒu�̐ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// �ʒu�̎擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// ��]�̐ݒ�
	void SetRotation(const DirectX::XMFLOAT4& rotation) { this->rotation = rotation; }

	// �p�x�̐ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle);

	// ��]�̎擾
	const DirectX::XMFLOAT4& GetRotation() const { return rotation; }

	// �p�x�̎擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// �p�x�̉��Z
	void AddAngle(const DirectX::XMFLOAT3& add_angle);
	
	// �p�x�̌��Z
	void SubtractAngle(const DirectX::XMFLOAT3& add_angle);

	// �X�P�[���̐ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// �X�P�[���̎擾
	const DirectX::XMFLOAT3 GetScale() const { return scale; }

	// �s��̎擾
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// �A�j���[�V�����t���O�ݒ�
	void SetAnimationFlag(bool animation_flag) { this->animation_flag = animation_flag; }
	// �A�j���[�V�����t���O�擾
	bool SetAnimationFlag() { return animation_flag; }
	// �R���|�[�l���g�ݒ�
	template<class T, class...Args>
	std::shared_ptr<T> AddShader(Args... args)
	{
		std::shared_ptr<T> shader = std::make_shared<T>(args...);
		this->shader = shader;
		return shader;
	}

	// �R���|�[�l���g�ݒ�
	template<class T, class...Args>
	std::shared_ptr<T> AddComponent(Args... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);
		component->SetActor(shared_from_this());
		components.emplace_back(component);
		return component;
	}

	// �R���|�[�l���g�擾
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for (std::shared_ptr<Component>& component : components)
		{
			std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(component);
			if (p == nullptr) continue;
			return p;
		}
		return nullptr;
	}
private:
	int						id;	// �G�l�~�[�f�[�^ID
	std::string				name;
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT4		rotation = { 0, 0, 0, 1 }; // �N�H�[�^�j�I���p
	DirectX::XMFLOAT3		angle = { 0, 0, 0 }; // �I�C���[�p�p
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4		transform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	std::unique_ptr<Model>  model;
	std::string				animation_node_origin;
	bool					animation_flag = true; // �A�j���[�V�����X�V���s�����̃t���O
	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<Component>>	components;
};