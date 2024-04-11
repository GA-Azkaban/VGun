#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D ShadowMap : register(t8);

float ShadowFactor(float4 worldPos)
{
	float4 lightSpacePosition = mul(worldPos, lightViewProjection);
	float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	float currentDepth = projCoords.z;
	if (currentDepth > 1)
		return 1.0f;
	projCoords = projCoords * 0.5 + 0.5;
	projCoords.y = 1.0 - projCoords.y;

	const float dx = 1.0f / screenWidth;
	const float dy = 1.0f / screenHeight;

	float2 offsets[9] =
	{
		float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
	};

	float shadow = 0;
	float bias = 0.0001f;
	
	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, 
			projCoords.xy + offsets[i], currentDepth - bias).r;
	}

	shadow /= 9.0f;
	return shadow;
}

#endif