
cbuffer cbPerObject
{
	float4x4 worldViewProj;
};

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

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.color = input.color;

	return output;
}