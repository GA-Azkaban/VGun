#include "Sampler.hlsli"

TextureCube Texture	: register(t0);

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 uv		: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 diffuse = Texture.SampleLevel(LinearWrapSampler, input.uv, 0.0);
	return float4(diffuse.rgb, 1.0f);
}
