#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#include "Sampler.hlsli"
#include "ConstantBuffer.hlsli"

Texture2D ShadowMap : register(t8);

//cbuffer LightProperties : register(b0)
//{
//	float4 cameraPosition;
//	float4 globalAmbient;
//	Light lights[MAX_LIGHTS];
//}

//cbuffer ShadowMapConstants : register(b1)
//{
//	float mapWidth;
//	float mapHeight;
//	float4x4 lightViewProjection;
//}

float ShadowFactor(float4 worldPos)
{
	float4 lightSpacePosition = mul(worldPos, lightViewProjection);
	float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	float currentDepth = projCoords.z;
	if (currentDepth > 1)
		return 1.0f;
	projCoords = projCoords * 0.5 + 0.5;
	projCoords.y = 1 - projCoords.y;

	float2 texelSize = float2(1, 1) / float2(screenWidth, screenHeight);

	float shadow = 0;
	float bias = 0.001f;
	for (int x = -1; x < 2; ++x)
	{
		for (int y = -1; y < 2; ++y)
		{			
			shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, projCoords.xy + float2(x, y) * texelSize, currentDepth - bias).r;
		}
	}

	shadow /= 9;
	return shadow;
}

#endif