#include "Sampler.hlsli"
#include "BRDF.hlsli"

cbuffer externalData : register(b0)
{
	float roughness;
}

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 uv	: TEXCOORD;
};

TextureCube CubeMap : register(t0);

static const uint SAMPLE_COUNT = 1024u;

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 N = normalize(input.uv);
	float3 V = N;

	float totalWeight = 0.0f;
	float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);
	for (uint i = 0u; i < SAMPLE_COUNT; ++i)
	{
		float2 st = Hammersley(i, SAMPLE_COUNT);
		float3 H = ImportanceSampleGGX(st, N, roughness);
		float3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = saturate(dot(N, L));
		if (NdotL > 0.0)
		{
			prefilteredColor += CubeMap.Sample(LinearSampler, L).rgb * NdotL;
			totalWeight += NdotL;
		}
	}
	prefilteredColor = prefilteredColor / totalWeight;

	return float4(prefilteredColor, 1.0f);
}