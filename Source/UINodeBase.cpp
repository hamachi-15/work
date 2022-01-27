#include "UINodeBase.h"
#include "UIActionBase.h"
#include "UIJudgmentBase.h"
#include "UIData.h"
#include "Sprite.h"

//------------------------
// デストラクタ
//------------------------
UINodeBase::~UINodeBase()
{
	delete action;
	//delete ui_data;
}

//------------------------
// 実行処理
//------------------------
void UINodeBase::Run(float elapsed_time)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (children.at(i)->action != NULL)
		{
			children.at(i)->action->Action(elapsed_time);
		}
		// 子ノードにさらに子ノードがあれば
		if (children.at(i)->GetChildren().size() > 0)
		{
			// 子ノードの子ノードの実行処理
			children.at(i)->Run(elapsed_time);
		}
	}
}

//------------------------
//描画処理
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
				Rebder(context, data->GetTexture(), data->GetPosition(), data->GetSize(), data->GetAngle());
			}
		}
		else
		{// 判定クラスがなければ無条件に描画
			if (children.at(i)->ui_data != NULL)
			{
				UIData* data = children.at(i)->ui_data;
				Rebder(context, data->GetTexture(), data->GetPosition(), data->GetSize(), data->GetAngle());
			}
		}
		// 子ノードにさらに子ノードがあれば
		if (children.at(i)->GetChildren().size() > 0)
		{
			// 子ノードの子ノードの描画処理
			children.at(i)->Render(context);
		}
	}
}

void UINodeBase::Rebder(ID3D11DeviceContext* context, 
	Texture* texture,
	const DirectX::XMFLOAT2& position, 
	const DirectX::XMFLOAT2& size, 
	const float& angle)
{
	Sprite sprite;
	sprite.Render(context,
		texture,
		position.x, position.y,
		size.x, size.y,
		0, 0,
		static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()));
}

//------------------------
// UIデータ検索
//------------------------
UINodeBase* UINodeBase::SearchNode(std::string search_name)
{
	// 名前が一致
	if (name == search_name)
	{
		return this;
	}
	else {
		// 子ノードで検索
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
// 子ノード取得
//------------------------
UINodeBase* UINodeBase::GetChild(int index)
{
	if (children.size() > index)
		return NULL;

	return children.at(index);
}

//------------------------
// 子ノード取得(末尾)
//------------------------
UINodeBase* UINodeBase::GetLastChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(children.size() - 1);
}

//------------------------
// 子ノード取得(先頭)
//------------------------
UINodeBase* UINodeBase::GetTopChild()
{
	if (children.size() == 0)
		return NULL;

	return children.at(0);
}
