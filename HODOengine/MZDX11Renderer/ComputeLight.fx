#include "LightHelper.fx"


cbuffer cbPerFrame
{
	DirectionalLight DirLights[3];
	PointLight PointLights[3];
	SpotLight SpotLights[3];
	float3 EyePosW;
};

cbuffer cbPerObject
{
	Material gMaterial;
};

SamplerState samAnisotropic
{
	Filter = MIN_MAG_MIP_LINEAR;

	//MaxAnisotropy = 4;

	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};


Texture2D PositionTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D DiffuseAlbedoTexture : register(t2);
Texture2D BumpedNormalTexture : register(t3);
Texture2D TangentTexture : register(t4);
//Texture2D ZBufferTexture		;	//: register(t3);


struct DEFERRED_VS_INPUT
{
	float3 Position	: POSITION;
	float2 Tex	: TEXCOORD;
};

struct DEFERRED_VS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 Tex	: TEXCOORD;
};

// ----------------------------------------------------------------------------
// Auxiliary method for sampling the G-Buffer attributes
void GetGBufferAttributes(float2 texCoord,
	out float3 normal,
	out float3 position,
	out float3 diffuseAlbedo)
	//out float3 specularAlbedo,
	//out float specularPower)
{
	// Sample normal texture
	normal = NormalTexture.Sample(samAnisotropic, float3(texCoord, 1.0f)).xyz;
	//normal = BumpedNormalTexture.Sample(samAnisotropic, float3(texCoord, 1.0f)).xyz;

	// Sample position texture
	position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;

	// Sample diffuse albedo
	diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord).xyz;

	// Sample specular albedo
	//float4 specularData = SpecularAlbedoTexture.Sample(samplerObj, texCoord);
	//specularAlbedo = specularData.xyz;
	//specularPower = specularData.w;
}

// ----------------------------------------------------------------------------
/*// Auxiliary method for calculating the lighing based on the sampled values
float3 CalculateLighting(int lightIndex,
	in float3 normal,
	in float3 position,
	in float3 diffuseAlbedo)
	//in float3 specularAlbedo,
	//in float specularPower)
{
	SpotLight currentLight = gSpotLights[lightIndex];

	// Light vector
	float3 LightVector = 0;

	// Calculate the light vector
	LightVector = currentLight.pos - position;

	// Calculate the attenuation based on the distance to the light source
	float dist = length(LightVector);
	// Normalize light vector
	LightVector /= dist;

	float d = max(0.0f, dist - currentLight.range);
	float denom = d / currentLight.range + 1.0f;
	float Attenuation = 1.0f / (denom * denom);
	float cutoff = 0.005f;
	Attenuation = (Attenuation - cutoff) / (1.0f - cutoff);
	Attenuation = max(0.0f, Attenuation);

	// Calculate the diffuse component
	float normalDotLight = saturate(dot(normal, LightVector));
	float3 diffuseComponent = currentLight.diffuse * diffuseAlbedo * normalDotLight;

	// Calculate the specular component
	//float3 SurfaceToCamera = CameraPosition - position;
	//float3 HalfVector = normalize(LightVector + SurfaceToCamera);
	//float3 specularComponent = specularAlbedo * currentLight.specular * normalDotLight * pow(saturate(dot(normal, HalfVector)), specularPower);

	// Calculate the final color
	//return saturate((diffuseComponent + specularComponent) * Attenuation);
	return saturate(diffuseComponent * Attenuation);
} */


// ----------------------------------------------------------------------------
// ComputeLighting
DEFERRED_VS_OUTPUT LightingVS(DEFERRED_VS_INPUT vin)
{
	DEFERRED_VS_OUTPUT output;

	//output.Position = mul(float4(vin.Position, 1.0f), gWorldViewProj);
	output.Position = float4(vin.Position, 1.0f);
	output.Tex = vin.Tex;

	return output;
}

float4 LightingPS(DEFERRED_VS_OUTPUT pin, uniform int gLightCount) : SV_TARGET
{
	float3 normal;
	float3 position;
	float3 diffuseAlbedo;
	//float3 specularAlbedo;
	//float specularPower;

	// Sample the GBuffer properties
	GetGBufferAttributes(pin.Tex, normal, position, diffuseAlbedo);
	//GetGBufferAttributes(input.TexCoord, normal, position, diffuseAlbedo, specularAlbedo, specularPower);

	// The toEye vector is used in lighting.
	float3 toEye = EyePosW - position;
	//float3 toEye = EyePosW - pin.Position;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	//
	// Lighting.
	//

	float4 texColor = float4(diffuseAlbedo, 1.0f);
	float4 litColor = texColor;
	if (gLightCount > 0)
	{
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, DirLights[i], normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;

			ComputePointLight(gMaterial, PointLights[i], position, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;

			ComputeSpotLight(gMaterial, SpotLights[i], position, normal, toEye,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		// Modulate with late add.
		litColor = texColor * (ambient + diffuse) + spec;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}


technique11 LightingTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, LightingVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, LightingPS(3)));
	}
}