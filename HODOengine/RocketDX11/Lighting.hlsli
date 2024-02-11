#ifndef LIGHTING_COMMON
#define LIGHTING_COMMON

#define MAX_LIGHTS 1
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_DISABLED 0
#define LIGHT_ENABLED 1
#define LIGHT_ENABLED_W_SHADOWMAP 2

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

#endif