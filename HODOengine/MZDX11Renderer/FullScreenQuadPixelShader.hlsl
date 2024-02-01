#include "Sampler.hlsli"
#include "Lighting.hlsli"

Texture2D DepthTexture      : register(t0);
Texture2D Diffuse       : register(t1);
Texture2D Normal        : register(t2);
//Texture2D MetalRough    : register(t3);
//Texture2D AO        : register(t4);

cbuffer lightData : register(b0)
{
	DirectionalLight dirLight;
	PointLight pointLight[4];
	SpotLight spotLight[2];

	float3 cameraPosition;
}

cbuffer externalData : register(b1)
{
	float4x4 inverseView;
	float4x4 inverseProjection;
}

struct VertexToPixel
{
	float4 position     : SV_POSITION;
	float2 uv           : TEXCOORD;
};

float3 CalculateWorldFromDepth(float depth, float2 texCoord)
{
	// clip space between [-1, 1]
	// flip y so that +ve y is upwards
	float2 clipXY = texCoord * 2.0 - 1.0;
	clipXY.y = -clipXY.y;

	// NOTE: depth is not linearized
	// Also in range [0, 1] due to DirectX Convention
	float4 clipSpace = float4(clipXY, depth, 1);
	float4 viewSpace = mul(inverseProjection, clipSpace);

	// perspective divide
	viewSpace /= viewSpace.w;

	float4 worldSpace = mul(inverseView, viewSpace);
	return worldSpace.xyz;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float depth = DepthTexture.Sample(PointSampler, input.uv).r;
	float3 posW = CalculateWorldFromDepth(depth, input.uv);
	float3 albedo = Diffuse.Sample(PointSampler, input.uv).rgb;
	float3 normal = Normal.Sample(PointSampler, input.uv).rgb;
	//float3 metalRough = MetalRough.Sample(PointSampler, input.uv);
	//float metallic = metalRough.r;
	//float roughness = metalRough.g;
	//float occlusion = metalRough.b;
	//float3 emissive = Emissive.Sample(PointSampler, input.uv).rgb;

	float3 toCamera = normalize(cameraPosition - posW);

	// Directional light calculation
	float dirLightAmount = saturate(dot(normal, -normalize(dirLight.Direction)));
	//float3 totalDirLight = dirLight.Color * dirLightAmount * albedo;
	float3 totalDirLight = dirLight.Color * dirLightAmount * 0.45f;
	float3 total = totalDirLight + albedo;
	return float4(total, 1.0f);
	//return float4(totalDirLight, 1.0f);
	//return float4(albedo, 1.0f);
}

