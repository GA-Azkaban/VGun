#include "Sampler.hlsli"

Texture2D CubeMap : register(t0);

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 uv	: TEXCOORD;
};

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
	float2 uv = float2(atan2(v.z, v.x), -asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 texCoord = SampleSphericalMap(normalize(input.uv));
	return float4(CubeMap.Sample(LinearWrapSampler, texCoord).rgb, 1.0f);
}