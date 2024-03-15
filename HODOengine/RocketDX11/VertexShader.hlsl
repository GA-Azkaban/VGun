#include "ConstantBuffer.hlsli"

//cbuffer externalData : register(b0)
//{
//	float4x4 world;
//	float4x4 worldViewProj;
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
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	float4 pos = mul(float4(input.position, 1.0f), world);
	output.position = mul(pos, viewProjection);
	output.normal = mul(input.normal, (float3x3)world);
	output.tangent = mul(input.tangent, (float3x3)world);
	output.worldPos = pos.xyz;
	output.uv = input.uv;

	return output;
}