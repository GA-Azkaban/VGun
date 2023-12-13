#pragma once
#include "IShader.h"

class VertexShader : public IShader
{
public:
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~VertexShader();
};


