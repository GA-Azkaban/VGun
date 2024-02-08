#include "Sampler.hlsli"
#include "Lighting.hlsli"
#include "Shading.hlsli"
#include "BRDF.hlsli"

//Texture2D DepthTexture      : register(t0);
Texture2D Position      : register(t0);
Texture2D Diffuse       : register(t1);
Texture2D Normal        : register(t2);
Texture2D MetalRough    : register(t3);
//Texture2D AO        : register(t4);
TextureCube EnvMap : register(t6);
TextureCube PrefilteredSpecMap : register(t7);
Texture2D BrdfLUT : register(t8);

cbuffer externalData : register(b0)
{
	float4x4 inverseView;
	float4x4 inverseProjection;
	int useEnvMap;
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
	//float depth = DepthTexture.Sample(PointSampler, input.uv).r;
	//float3 posW = CalculateWorldFromDepth(depth, input.uv);
	float3 posW = Position.Sample(PointSampler, input.uv).rgb;
	float3 albedo = Diffuse.Sample(PointSampler, input.uv).rgb;
	float3 normal = Normal.Sample(PointSampler, input.uv).rgb;
	float3 metalRough = MetalRough.Sample(PointSampler, input.uv);
	float metallic = metalRough.r;
	float roughness = metalRough.g;
	float occlusion = metalRough.b;
	//float3 emissive = Emissive.Sample(PointSampler, input.uv).rgb;

	SurfaceInfo surf;
	surf.posW = float4(posW, 1.0f);
	surf.N = normal;
	surf.V = normalize(cameraPosition.xyz - posW);
	surf.NdotV = dot(surf.N, surf.V);

	float3 Lo = float3(0, 0, 0);
	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, albedo, metallic);

	// Light calculation
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		Light light = lights[i];
		if (light.status == LIGHT_DISABLED)
			continue;
		LightingInfo li = EvalLightingInfo(surf, light);

		// cook-torrance brdf
		float NDF = DistributionGGX(saturate(li.NdotH), roughness);
		float G = GeometrySmith(saturate(surf.NdotV), saturate(li.NdotL), roughness);
		float3 F = fresnelSchlick(saturate(dot(li.H, surf.V)), F0);
		float3 kS = F;
		float3 kD = float3(1.0, 1.0, 1.0) - kS;
		kD *= 1.0 - metallic;

		float NdotL = saturate(li.NdotL);

		float3 numerator = NDF * G * F;
		float denominator = 4.0 * saturate(surf.NdotV) * NdotL;
		float3 specular = numerator / max(denominator, 0.001);

		Lo += (kD * albedo / PI + specular) * light.color.rgb * li.attenuation * NdotL * li.shadowFactor;
	}

	float3 ambient = globalAmbient.rgb * albedo;

	if (useEnvMap)
	{
		float3 kS = fresnelSchlickRoughness(saturate(surf.NdotV), F0, roughness);
		float3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
		float3 irradiance = EnvMap.Sample(LinearSampler, surf.N).rgb;
		float3 diffuse = irradiance * albedo;

		float3 R = normalize(reflect(-surf.V, surf.N));
		float3 prefilteredColor = PrefilteredSpecMap.SampleLevel(LinearSampler, R, roughness * 5.0).rgb;
		float2 envBrdf = BrdfLUT.Sample(PointSampler, float2(saturate(surf.NdotV), roughness)).rg;
		float3 specular = prefilteredColor * (kS * envBrdf.x + envBrdf.y);
		ambient = (kD * diffuse + specular);
	}

	ambient *= occlusion;
	float3 color = ambient + Lo;
	return float4(color, 1.0f);
}

