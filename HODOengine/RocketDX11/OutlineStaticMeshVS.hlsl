
#include "ConstantBuffer.hlsli"

//cbuffer externalData : register(b0)
//{
//	float4x4 worldViewProj;
//	float4 color;
//}

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
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
	output.color = outlineColor;

	return output;
}