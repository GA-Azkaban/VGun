#include "Sampler.hlsli"
#include "Shading.hlsli"

Texture2D Albedo : register(t0);
Texture2D NormalMap : register(t1);
Texture2D OcclusionRoughnessMetal : register(t2);
Texture2D Roughness : register(t3);
Texture2D Metallic : register(t4);

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
	float4 occlusionRoughMetal : SV_TARGET3;
};

PSOutput main(VertexToPixel input)
{
	PSOutput output;

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Read and unpack normal from map
	if (useNormalMap)
	{
		float3 normalFromMap = NormalMap.Sample(LinearWrapSampler, input.uv).xyz * 2 - 1;

		// Transform from tangent to world space
		float3 N = input.normal;
		float3 T = normalize(input.tangent - N * dot(input.tangent, N));
		float3 B = cross(T, N);

		float3x3 TBN = float3x3(T, B, N);
		input.normal = normalize(mul(normalFromMap, TBN));
	}

	// Sample the texture
	float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (useAlbedo)
	{
		textureColor = Albedo.Sample(LinearWrapSampler, input.uv);
	}

	float occlusion = 1.0f;

	float metallic = metallicValue;
	float roughness = roughnessValue;

	if (useOccMetalRough)
	{
		float3 occRoughMetal = OcclusionRoughnessMetal.Sample(LinearWrapSampler, input.uv).rgb;
		occlusion = occRoughMetal.r;
		roughness = occRoughMetal.g;
		metallic = occRoughMetal.b;
	}

	if (useRoughnessMap)
	{
		roughness = Roughness.Sample(LinearWrapSampler, input.uv).r;
	}

	if (useMetallicMap)
	{
		metallic = Metallic.Sample(LinearWrapSampler, input.uv).r;
	}

	output.position = float4(input.worldPos, 1.0f);
	output.diffuse = textureColor * albedoColor;
	//output.diffuse = pow(float4(textureColor.rgb, 0), 2.2f);
	//output.normal = float4(input.normal, 1.0f);
	output.normal = float4(input.normal * 0.5f + 0.5f, 1.0f);
	output.occlusionRoughMetal = float4(occlusion, roughness, metallic, 1.0f);

	return output;
}