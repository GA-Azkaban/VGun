#include "Sampler.hlsli"

Texture2D OutlineTexture : register(t0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 color = OutlineTexture.Sample(PointClampSampler, input.uv).rgb;
	
	return float4(color, 1.0f);
}