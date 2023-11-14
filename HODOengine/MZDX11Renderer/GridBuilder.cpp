#include "GridBuilder.h"

IBuilder* GridBuilder::SetDevice(ID3D11Device* pDevice)
{
	m_d3dDevice = pDevice;
	return this;
}

IBuilder* GridBuilder::SetDeviceContext(ID3D11DeviceContext* pDeviceContext)
{
	m_d3dImmediateContext = pDeviceContext;
	return this;
}

IBuilder* GridBuilder::SetRasterizerState(ID3D11RasterizerState* pRS)
{
	m_pRS = pRS;
	return this;
}

IRenderableObject* GridBuilder::Build()
{
	/*IRenderableObject* grid = new Grid();

	grid->SetDevice(m_d3dDevice);
	grid->SetDeviceContext(m_d3dImmediateContext);
	grid->SetRasterizerState(m_pRS);

	return grid;*/
	return nullptr;
}
