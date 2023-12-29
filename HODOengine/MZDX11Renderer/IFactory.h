#pragma once
#include <d3d11.h>

/// <summary>
/// Factory 인터페이스.
/// 오브젝트를 찍어내는 공장과 같은 역할을 한다.
/// 2023.07.04 MJKIM
/// </summary>

class IBuilder;
class Grid;

class IFactory
{
public:
	IFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~IFactory();


	/// 오브젝트마다 매개변수를 다르게 가지고 있을 경우에는
	/// 어떻게 처리해야 하는가?	
	template <class T>
	T* CreateObject()
	{
		return new T(m_d3dDevice, m_d3dImmediateContext, m_pRS);
	}
	/// 해결방법 : 템플릿 특수화
	/// 그렇지만 이 방법은 템플릿 특수화로 매번 특수한 케이스에 맞는 함수를 
	/// 하나씩 만들어야 되는 것 같은데...


private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;
	ID3D11RasterizerState* m_pRS;
};