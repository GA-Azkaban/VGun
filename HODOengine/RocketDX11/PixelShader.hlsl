#include "Sampler.hlsli"

cbuffer externalData : register(b0)
{
	float gMetallic;
	float gRoughness;

	int useNormalMap;
	int useOccMetalRough;
}

Texture2D Albedo : register(t0);
Texture2D NormalMap : register(t1);
Texture2D OcclusionRoughnessMetal : register(t2);
//Texture2D Emissive : register(t3);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

struct PSOutput
{
	float4 position : SV_TARGET0;
	float4 diffuse : SV_TARGET1;
	float4 normal : SV_TARGET2;
	float4 metalRoughOcclusion : SV_TARGET3;
	//float4 emissive : SV_TARGET4;
};

PSOutput main(VertexToPixel input)
{
	PSOutput output;

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Read and unpack normal from map
	if (useNormalMap)
	{
		float3 normalFromMap = NormalMap.Sample(LinearSampler, input.uv).xyz * 2 - 1;

		// Transform from tangent to world space
		float3 N = input.normal;
		float3 T = normalize(input.tangent - N * dot(input.tangent, N));
		float3 B = cross(T, N);

		float3x3 TBN = float3x3(T, B, N);
		input.normal = normalize(mul(normalFromMap, TBN));
	}

	// Sample the texture
	float4 textureColor = Albedo.Sample(LinearSampler, input.uv);

	float occlusion = 1.0f;

	float metallic = gMetallic;
	float roughness = gRoughness;

	if (useOccMetalRough)
	{
		float3 occRoughMetal = OcclusionRoughnessMetal.Sample(LinearSampler, input.uv).rgb;
		occlusion = occRoughMetal.r;
		roughness = occRoughMetal.g;
		metallic = occRoughMetal.b;
	}

	//float4 emissive = Emissive.Sample(LinearSampler, input.uv);

	output.position = float4(input.worldPos, 1.0f);
	output.diffuse = textureColor;
	output.normal = float4(input.normal, 1.0f);
	output.metalRoughOcclusion.r = metallic;
	output.metalRoughOcclusion.g = roughness;
	output.metalRoughOcclusion.b = occlusion;
	//output.emissive = emissive;

	return output;
}