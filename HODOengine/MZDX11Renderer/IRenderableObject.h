#pragma once
#include <DirectXMath.h>

/// <summary>
/// Renderable Object�� �������̽�.
/// IMeshObject, IUIObject �� �׷����� ����� �������̽��� ��ӵȴ�.
/// 2023.07.04 MJKIM
/// </summary>

class IRenderableObject
{
public:
	virtual ~IRenderableObject() {};

	virtual void SetWorldTM(const DirectX::XMMATRIX& tm) = 0;
	virtual void SetActive(bool isActive) = 0;
};