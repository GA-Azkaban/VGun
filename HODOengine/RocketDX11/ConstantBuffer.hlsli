
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
}

cbuffer MaterialData : register(b5)
{
	float gMetallic;
	float gRoughness;
	
	int useAlbedo;
	int useNormalMap;
	int useOccMetalRough;
	int useEnvMap;

	float4 outlineColor;
}

cbuffer PostProcessData : register(b6)
{
	float4 ssaoSamples[64];
	float2 ssaoNoiseScale;
	float ssaoRadius;
	float ssaoPower;
}

#endif