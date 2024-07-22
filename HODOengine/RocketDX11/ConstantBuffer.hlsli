
#ifndef _CONSTANTBUFFER_
#define _CONSTANTBUFFER_

#include "CommonData.hlsli"

#define MAX_LIGHTS 1
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

cbuffer PerFrame : register(b0)
{
	float4x4 view;
	float4x4 projection;
	float4x4 inverseProjection;
	float4x4 viewProjection;
	float4 cameraPosition;
	float screenWidth;
	float screenHeight;
}

cbuffer PerObject : register(b1)
{
	float4x4 world;
}

cbuffer SkeletonData : register(b2)
{
	float4x4 boneTransforms[96];
}

cbuffer LightData : register(b3)
{
	float4 globalAmbient;
	Light lights[MAX_LIGHTS];
}

cbuffer ShadowData : register(b4)
{
	float4x4 lightViewProjection;
	int shadowMapSize;
}

cbuffer MaterialData : register(b5)
{
	float4 albedoColor;

	float4 outlineColor;
	float outlineThreshHold;
	float outlineThickness;

	float metallicValue;
	float roughnessValue;
	
	int useAlbedo;
	int useNormalMap;
	int useOccMetalRough;
	int useRoughnessMap;
	int useMetallicMap;
    int useMaskMap;
	int useEnvMap;
}

cbuffer EnvironmentData : register(b6)
{
	float envLightIntensity;
}

cbuffer PostProcessData : register(b7)
{
	float4 ssaoSamples[64];
	float2 ssaoNoiseScale;
	float ssaoRadius;
	float ssaoPower;
    int vignetteEnabled;
	float vignetteIntensity;
}

cbuffer ParticleData : register(b8)
{
    float4x4 particleTransforms[512];
    float4 particleColors[512];
}

#endif