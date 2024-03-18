
#include "ConstantBuffer.hlsli"

//cbuffer cbPerObject
//{
//	float4x4 worldViewProj;
//};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	float4 pos = mul(float4(input.position, 1.0f), world);
	output.position = mul(pos, viewProjection);
	output.color = input.color;

	return output;
}