#include "Sampler.hlsli"

TextureCube Texture	: register(t0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uv			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(Texture.SampleLevel(LinearSampler, input.uv, 0.0).rgb, 1);
}

