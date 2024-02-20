
cbuffer externalData : register(b0)
{
	float4x4 worldViewProj;
};

struct VertexShaderInput
{
	float3 position	: POSITION;
};

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 worldPos	: POSITION;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane)
	output.position = mul(float4(input.position, 1.0f), worldViewProj).xyww;

	// Use local vertex position as cubemap lookup vector.
	output.worldPos = input.position;

	return output;
}