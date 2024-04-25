#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D ShadowMap : register(t8);

float ShadowFactor(float4 worldPos, float nDotL)
{
	float4 lightSpacePosition = mul(worldPos, lightViewProjection);
	float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	float currentDepth = projCoords.z;
	if (currentDepth > 1)
		return 1.0f;
	projCoords = projCoords * 0.5 + 0.5;
	projCoords.y = 1.0 - projCoords.y;

	float2 texelSize = float2(1, 1) / float2(screenWidth, screenHeight);

	float shadow = 0;
	float bias = 0.001f;
    //float bias = max(0.00005 * (1.0 - nDotL), 0.000005f);

	for (int x = -2; x < 3; ++x)
	{
		for (int y = -2; y < 3; ++y)
		{
			shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler,
				projCoords.xy + float2(x, y) * texelSize, currentDepth - bias).r;
		}
	}

	shadow /= 25.0f;
	return shadow;
}

#endif