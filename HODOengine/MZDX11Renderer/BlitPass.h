#pragma once

class QuadBuffer;
class VertexShader;
class PixelShader;
class MZDX11Renderer;

class BlitPass
{
public:
	BlitPass(QuadBuffer* quadBuffer);
	~BlitPass();

	void Render();

private:
	QuadBuffer* _quadBuffer;
	VertexShader* _vertexShader;
	PixelShader* _pixelShader;

	MZDX11Renderer& _mzRenderer;
};

