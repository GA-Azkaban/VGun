#pragma once
#include <DirectXMath.h>

/// <summary>
/// Renderable Object의 인터페이스.
/// IMeshObject, IUIObject 등 그려지는 대상의 인터페이스에 상속된다.
/// 2023.07.04 MJKIM
/// </summary>

class IRenderableObject
{
public:
	virtual ~IRenderableObject() {};

	virtual void SetWorldTM(const DirectX::XMMATRIX& tm) = 0;
	virtual void SetActive(bool isActive) = 0;
};