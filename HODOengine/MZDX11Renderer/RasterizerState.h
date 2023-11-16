#pragma once
#include "DX11Define.h"

class RasterizerState
{
public:
	static void CreateRenderStates(ID3D11Device* device);
	static void DestroyRenderStates();

	static ComPtr<ID3D11RasterizerState> WireframeRS;
	static ComPtr<ID3D11RasterizerState> SolidRS;
};

