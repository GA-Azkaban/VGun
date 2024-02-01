#include "Sampler.hlsli"

TextureCube Texture	: register(t0);

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 worldPos	: POSITION;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return Texture.Sample(LinearSampler, input.worldPos);
}
