#pragma once
#pragma once
#include "../HODOengine/HODO_API.h"

class UITransformController : public HDData::Script
{
public:
	void Start() override
	{
		_imageUI = GetGameObject()->GetComponent<HDData::ImageUI>();
	}

	void Update() override
	{
		Vector3 pos = GetGameObject()->GetTransform()->GetLocalPosition();

		if (API::GetKeyDown(DIK_1))
		{
			GetGameObject()->GetTransform()->SetLocalPosition(pos.x + 0.5f, pos.y, pos.z);
		}
		if (API::GetKeyDown(DIK_2))
		{
			GetGameObject()->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y, pos.z);
		}

		if (API::GetKeyDown(DIK_3))
		{
			GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y + 0.5f, pos.z);
		}
		if (API::GetKeyDown(DIK_4))
		{
			GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y - 0.5f, pos.z);
		}

		DirectX::XMFLOAT2 scale = _imageUI->GetScale();

		if (API::GetKeyDown(DIK_Z))
		{
			scale.x += 0.1f;
		}
		if (API::GetKeyDown(DIK_X))
		{
			scale.x -= 0.1f;
		}

		if (API::GetKeyDown(DIK_C))
		{
			scale.y += 0.1f;
		}
		if (API::GetKeyDown(DIK_V))
		{
			scale.y -= 0.1f;
		}

		_imageUI->ChangeScale(scale.x, scale.y);
	}

private:
	HDData::ImageUI* _imageUI;
};
