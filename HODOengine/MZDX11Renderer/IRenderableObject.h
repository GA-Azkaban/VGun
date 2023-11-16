#pragma once
#include "DX11Define.h"
#include "d3dx11effect.h"

/// <summary>
/// Renderable Object의 인터페이스.
/// 2023.07.04 MJKIM
/// 
/// Deferred Rendering을 위해 구조를 변경하였다.
/// Render 부분은 DeferredRenderer에서 RenderAll()로 그려줄 것임.
/// 2023.07.19 MJKIM
/// 
/// IRenderableObject은 모든 Object들의 부모로 만들고,
/// IMeshObject, IUIObject 등에 상속해줄 것이다.
/// 그러기 위해 최소한의 기능만 남겨두고 모두 지웠다.
/// 2023.08.16 MJKIM
/// </summary>

class MZCamera;

class IRenderableObject
{
public:
	virtual ~IRenderableObject() {};

	virtual void SetWorldTM(const XMMATRIX& tm) abstract;
	virtual void SetActive(bool isActive) abstract;

protected:
	//bool m_isActive = true;
};