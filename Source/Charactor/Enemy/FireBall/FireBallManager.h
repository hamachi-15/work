#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "FireBall.h"
class Actor;

class FireBallManager
{
private:
	FireBallManager() {}
	~FireBallManager() {}
public:
	// �C���X�^���X�擾
	static FireBallManager& Instance()
	{
		static FireBallManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);
	
	// �`�揈��
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// �΋��쐬
	std::shared_ptr<FireBall> Create(std::shared_ptr<Actor> actor, DirectX::XMFLOAT3 direction);

	// �΋��폜
	void Unregister(std::shared_ptr<FireBall> fireball);

	// �j������
	void Destroy();
private:
	std::vector<std::shared_ptr<FireBall>> start_fireballs;
	std::vector<std::shared_ptr<FireBall>> update_fireballs;
	std::vector<std::shared_ptr<FireBall>> remove_fireballs;
};