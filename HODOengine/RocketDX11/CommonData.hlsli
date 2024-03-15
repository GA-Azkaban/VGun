#ifndef _COMMON_DATA_
#define _COMMON_DATA_

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

#endif