#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D DepthStencilTex	: register(t0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return outlineColor;
}