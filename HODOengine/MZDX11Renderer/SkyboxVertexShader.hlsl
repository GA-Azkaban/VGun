
cbuffer externalData : register(b0)
{
	float4x4 world;
	float4x4 worldViewProj;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	output.uv = input.position;

	return output;
}