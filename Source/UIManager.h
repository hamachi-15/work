#pragma once
#include <memory>
#include <vector>
#include <d3d11.h>
#include <string>

class UI;
//******************************
// 
// UI�}�l�[�W���[
// 
//******************************
class UIManager
{
private:
	// �R���X�g���N�^
	UIManager();
	// �f�X�g���N�^
	~UIManager();
public:
	// �C���X�^���X�擾
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Draw(ID3D11DeviceContext* context);

	// UI�o�^
	void RegisterUI(std::shared_ptr<UI> ui);

	// UI�폜
	void UnRegisterUI(std::shared_ptr<UI> ui);

	// �S�폜
	void AllDelete();

	// UI�擾
	std::shared_ptr<UI> GetUI(std::string name);
private:
	std::vector<std::shared_ptr<UI>> ui_starts;
	std::vector<std::shared_ptr<UI>> ui_updates;
	std::vector<std::shared_ptr<UI>> ui_removes;
};