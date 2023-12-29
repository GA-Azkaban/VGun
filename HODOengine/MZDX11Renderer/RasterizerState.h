#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

class RasterizerState
{
public:
	static LazyObjects<RasterizerState> Instance;
	friend LazyObjects<RasterizerState>;

	void CreateRenderStates(ID3D11Device* device);
	void DestroyRenderStates();

	ID3D11RasterizerState* GetWireframeRS() { return m_wireframeRS.Get(); }
	ID3D11RasterizerState* GetSolidRS() { return m_solidRS.Get(); }

private:
	RasterizerState();

	ComPtr<ID3D11RasterizerState> m_wireframeRS;
	ComPtr<ID3D11RasterizerState> m_solidRS;
};

