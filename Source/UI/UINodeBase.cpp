#include "UINodeBase.h"
#include "UIActionBase.h"
#include "UIJudgmentBase.h"
#include "UIData.h"
#include "Sprite.h"

//------------------------
// �f�X�g���N�^
//------------------------
UINodeBase::~UINodeBase()
{
	delete action;
	delete judgment;
}
//------------------------
// ���s����
//------------------------
void UINodeBase::Run(float elapsed_time)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (children.at(i)->action != NULL)
		{
			children.at(i)->action->Action(elapsed_time);
		}
		// �q�m�[�h�ɂ���Ɏq�m�[�h�������
		if (children.at(i)->GetChildren().size() > 0)
		{
			// �q�m�[�h�̎q�m�[�h�̎��s����
			children.at(i)->Run(elapsed_time);
		}
	}
}

//------------------------
//�`�揈��
//------------------------
void UINodeBase::Render(ID3D11DeviceContext* context)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (children.at(i)->judgment != NULL)
		{
			if (children.at(i)->judgment->Judgment() == true)
			{
				UIData* data = children.at(i)->ui_data;
				Rebder(context, data->GetTexture(), data->GetPosition(), data->GetSize(), data->GetScale(), data->GetAngle());
			}
		}
		else
		{// ����N���X���Ȃ���Ζ������ɕ`��
			if (children.at(i)->ui_data != NULL)
			{
				UIData* data = children.at(i)->ui_data;
				Rebder(context, data->GetTexture(), data->GetPosition(), data->GetSize(), data->GetScale(), data->GetAngle());
			}
		}
		// �q�m�[�h�ɂ���Ɏq�m�[�h�������
		if (children.at(i)->GetChildren().size() > 0)
		{
			// �q�m�[�h�̎q�m�[�h�̕`�揈��
			children.at(i)->Render(context);
		}
	}
}

void UINodeBase::Rebder(ID3D11DeviceContext* context, 
	Texture* texture,
	const DirectX::XMFLOAT2& position, 
	const DirectX::XMFLOAT2& size, 
	const DirectX::XMFLOAT2& scale,
	const float& angle)
{
	float texture_width = static_cast<float>(texture->GetWidth());
	float texture_height = static_cast<float>(texture->GetHeight());
	Sprite sprite;
	sprite.Render(context,
		texture,
		position.x, position.y,
		size.x * scale.x, size.y * scale.y,
		0, 0,
		texture_width, texture_height,
		angle);
}

//------------------------
// UI�f�[�^����
//------------------------
UINodeBase* UINodeBase::SearchNode(std::string search_name)
{
	// ���O����v
	if (name == search_name)
	{
		return this;
	}
	else {
		// �q�m�[�h�Ō���
		for (auto itr = children.begin(); itr != children.end(); itr++)
		{
			UINodeBase* ret = (*itr)->SearchNode(search_name);

			if (ret != NULL)
			{
				return ret;
			}
		}
	}

	return NULL;
}

//------------------------
// �q�m�[�h�擾
//------------------------
UINodeBase* UINodeBase::GetChild(int index)
{
	if (children.size() > index)
		return NULL;

	return children.at(index);
}

//------------------------
// �q�m�[�h�擾(����)
//------------------------
UINodeBase* UINodeBase::GetLastChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(children.size() - 1);
}

//------------------------
// �q�m�[�h�擾(�擪)
//------------------------
UINodeBase* UINodeBase::GetTopChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(0);
}
