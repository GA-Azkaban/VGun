#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#include "Sampler.hlsli"

#define MAX_LIGHTS 1
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

Texture2D ShadowMap : register(t8);

struct Light
{
	//float4 position;
	float4 direction;
	float4 color;

	//float range;
	//float spotAngle;

	int lightType;
	int status;
};

cbuffer LightProperties : register(b0)
{
	float4 cameraPosition;
	float4 globalAmbient;
	Light lights[MAX_LIGHTS];
}

cbuffer ShadowMapConstants : register(b1)
{
	float mapWidth;
	float mapHeight;
	float4x4 lightViewProjection;
}

float ShadowFactor(float4 worldPos)
{
	float4 lightSpacePosition = mul(worldPos, lightViewProjection);
	float3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	float currentDepth = projCoords.z;
	if (currentDepth > 1)
		return 1.0f;
	projCoords = projCoords * 0.5 + 0.5;
	projCoords.y = 1 - projCoords.y;
	//projCoords = (projCoords + 1) / 2.0; // change to [0 - 1]
	//projCoords.y = -projCoords.y; // bottom right corner if (1, -1) in NDC so we have to flip it

	float2 texelSize = float2(1, 1) / float2(mapWidth, mapHeight);

	float shadow = 0;
	float bias = 0.001f;
	for (int x = -1; x < 2; ++x)
	{
		for (int y = -1; y < 2; ++y)
		{
			//float closestDepth = ShadowMap.Sample(LinearSampler, projCoords.xy + float2(x, y) * texelSize).r;
			//shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
			shadow += ShadowMap.SampleCmpLevelZero(ShadowSampler, projCoords.xy + float2(x, y) * texelSize, currentDepth - bias).r;
		}
	}

	shadow /= 9;
	return shadow;
}

#endif