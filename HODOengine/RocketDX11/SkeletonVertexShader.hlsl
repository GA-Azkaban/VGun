
cbuffer externalData : register(b0)
{
	float4x4 world;
	float4x4 worldViewProj;
}

cbuffer skeleton
{
	float4x4 boneTransforms[96];
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
	float4 weights		: BLENDWEIGHT;
	uint4 boneIndices	: BLENDINDICES;
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

	float _weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	_weights[0] = input.weights.x;
	_weights[1] = input.weights.y;
	_weights[2] = input.weights.z;
	_weights[3] = input.weights.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		posL += _weights[i] * mul(float4(input.position, 1.0f), boneTransforms[input.boneIndices[i]]).xyz;
		normalL += _weights[i] * mul(input.normal, (float3x3)boneTransforms[input.boneIndices[i]]);
	}

	output.worldPos = mul(float4(posL, 1.0f), world).xyz;
	output.normal = mul(normalL, (float3x3)world);

	output.position = mul(float4(posL, 1.0f), worldViewProj);
	output.tangent = mul(input.tangent, (float3x3)world);
	output.uv = input.uv;

	return output;
}