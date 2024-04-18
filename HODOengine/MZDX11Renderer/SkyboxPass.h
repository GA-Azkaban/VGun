#pragma once

class DeferredBuffers;
class QuadBuffer;
class VertexShader;
class PixelShader;

class SkyboxPass
{
public:
	SkyboxPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);

	void Render();

private:
	DeferredBuffers* _deferredBuffers;
	QuadBuffer* _quadBuffer;
};

