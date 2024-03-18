#include "ConstantBuffer.hlsli"

struct VertexShaderInput
{
	float3 position	: POSITION;
};

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 uv	: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane)
	float4 pos = mul(float4(input.position, 1.0f), world);
	output.position = mul(pos, viewProjection).xyww;

	// Use local vertex position as cubemap lookup vector.
	output.uv = input.position;

	return output;
}