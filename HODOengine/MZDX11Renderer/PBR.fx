#include "PBRLightHelper.fx"


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
{
	// Sample normal texture
	//normal = NormalTexture.Sample(samAnisotropic, float3(texCoord, 1.0f)).xyz;
	normal = BumpedNormalTexture.Sample(samAnisotropic, float3(texCoord, 1.0f)).xyz;

	// Sample position texture
	position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;

	// Sample diffuse albedo
	diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord).xyz;

}


// ----------------------------------------------------------------------------
// ComputeLighting
DEFERRED_VS_OUTPUT LightingVS(DEFERRED_VS_INPUT vin)
{
	DEFERRED_VS_OUTPUT output;

	output.Position = float4(vin.Position, 1.0f);
	output.Tex = vin.Tex;

	return output;
}

float4 LightingPS(DEFERRED_VS_OUTPUT pin, uniform int gLightCount) : SV_TARGET
{
	float3 normal;
	float3 position;
	float3 diffuseAlbedo;

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
		float3 directLighting = float3(0.0f, 0.0f, 0.0f);
		float3 Rad = float3(0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(DirLights[i], normal, diffuseAlbedo, 
				-toEye, Rad);

			directLighting += Rad;

			ComputePointLight(PointLights[i], position, normal, diffuseAlbedo, 
				-toEye, Rad);

			directLighting += Rad;

			ComputeSpotLight(SpotLights[i], position, normal, diffuseAlbedo,
				-toEye, Rad);

			directLighting += Rad;
		}

		// Modulate with late add.
		litColor = float4(directLighting, 1.0f);
	}

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