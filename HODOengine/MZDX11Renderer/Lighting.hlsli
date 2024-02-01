#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

struct DirectionalLight
{
	float4 Color;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float4 Position;
};

struct SpotLight
{
	float4 Color;
	float4 Position;
	float3 Direction;
	float SpotPower;
};

#endif