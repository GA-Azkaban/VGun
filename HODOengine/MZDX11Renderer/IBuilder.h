#pragma once
#include "IRenderableObject.h"

/// <summary>
/// Builder 인터페이스.
/// Object들의 Builder에 상속
/// 2023.07.04 MJKIM
/// </summary>

class IRenderableObject;

class IBuilder
{
public:
	virtual IBuilder* SetDevice(ID3D11Device* pDevice) abstract;
	virtual IBuilder* SetDeviceContext(ID3D11DeviceContext* pDeviceContext) abstract;
	virtual IBuilder* SetRasterizerState(ID3D11RasterizerState* pRS) abstract;

	virtual IRenderableObject* Build() abstract;

protected:
	IRenderableObject* m_pObject;
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;
	ID3D11RasterizerState* m_pRS;
};