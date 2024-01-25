#pragma once
#include <d3d11.h>

class DeferredBuffers;
class QuadBuffer;
class VertexShader;
class PixelShader;

class DeferredPass
{
public:
	DeferredPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);

	void Render();

private:
	DeferredBuffers* _deferredBuffers;
	QuadBuffer* _quadBuffer;

	VertexShader* _vertexShader;
	PixelShader* _pixelShader;
};

