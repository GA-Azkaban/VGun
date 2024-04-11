#include "Shading.hlsli"
#include "BRDF.hlsli"

Texture2D Position      : register(t0);
Texture2D Albedo       : register(t1);
Texture2D Normal        : register(t2);
Texture2D OcclusionRoughnessMetal    : register(t3);
Texture2D AO        : register(t4);
TextureCube EnvMap : register(t5);
TextureCube PrefilteredSpecMap : register(t6);
Texture2D BrdfLUT : register(t7);

struct VertexToPixel
{
	float4 position     : SV_POSITION;
	float2 uv           : TEXCOORD;
};

float3 GetViewSpacePositionFromDepth(float2 texCoord, float depth)
{
	float4 clipSpaceLocation;
	clipSpaceLocation.xy = texCoord * 2.0f - 1.0f;
	clipSpaceLocation.y *= -1;
	clipSpaceLocation.z = depth;
	clipSpaceLocation.w = 1.0f;
	float4 homogenousLocation = mul(clipSpaceLocation, inverseProjection);
	return homogenousLocation.xyz / homogenousLocation.w;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	//float depth = DepthTexture.Sample(PointClampSampler, input.uv).r;
	//float3 viewPosition = GetViewSpacePositionFromDepth(input.uv, depth);
	float3 posW = Position.Sample(PointClampSampler, input.uv).rgb;
	float3 albedo = Albedo.Sample(PointClampSampler, input.uv).rgb;
	float3 normal = Normal.Sample(PointClampSampler, input.uv).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	float3 metalRough = OcclusionRoughnessMetal.Sample(PointClampSampler, input.uv).rgb;
	float occlusion = metalRough.r;
	float roughness = metalRough.g;
	float metallic = metalRough.b;

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
		float3 irradiance = EnvMap.Sample(LinearWrapSampler, surf.N).rgb;
		irradiance *= envLightIntensity;
		float3 diffuse = irradiance * albedo;

		float3 R = normalize(reflect(-surf.V, surf.N));
		float3 prefilteredColor = PrefilteredSpecMap.SampleLevel(LinearWrapSampler, R, roughness * 5.0).rgb;
		float2 envBrdf = BrdfLUT.Sample(PointClampSampler, float2(saturate(surf.NdotV), roughness)).rg;
		float3 specular = envLightIntensity * prefilteredColor * (kS * envBrdf.x + envBrdf.y);
		//float3 specular = prefilteredColor * (kS * envBrdf.x + envBrdf.y);
		ambient = (kD * diffuse + specular);
	}

	float ao = AO.Sample(PointClampSampler, input.uv).r;
	ambient *= ao * occlusion;

	float3 color = ambient + Lo;
	return float4(color, 1.0f);
}

