#pragma once
#include "IBuilder.h"
#include "Grid.h"
#include "IRenderableObject.h"

/// <summary>
/// IBuilder를 상속받는 GridBuilder 클래스.
/// 이렇게 오브젝트마다 Builder를 하나씩 만들어야 하는 것은
/// 불합리한 것 같은데...
/// 일단 만들어보자.
/// 2023.07.04 MJKIM
/// </summary>

class GridBuilder : public IBuilder
{
public:
	virtual IBuilder* SetDevice(ID3D11Device* pDevice) override;
	virtual IBuilder* SetDeviceContext(ID3D11DeviceContext* pDeviceContext) override;
	virtual IBuilder* SetRasterizerState(ID3D11RasterizerState* pRS) override;

	virtual IRenderableObject* Build() override;
};