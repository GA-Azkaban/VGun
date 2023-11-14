#include "LightHelper.fx"


cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;

SamplerState samAnisotropic
{
	Filter = MIN_MAG_MIP_LINEAR;

	//MaxAnisotropy = 4;

	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

struct VS_INPUT		// Basic Tech
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
	float2 Tex		: TEXCOORD;
	float3 TangentL	: TANGENT;
};

struct VS_OUTPUT	// Basic Tech
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float3 TangentW	: TANGENT;
	float2 Tex		: TEXCOORD;
	//float4 Color   : COLOR;
};


struct PS_OUTPUT	// Basic Tech
{
	float4 Position			: SV_Target0; // Render target 0
	float4 Normal			: SV_Target1; // Render target 1
	float4 DiffuseAlbedo	: SV_Target2; // Render target 2
	float4 BumpedNormalW	: SV_Target3;
	//float4 ZBuffer		: SV_Target3; // Render target 3
	float4 Tangent			: SV_Target4; // Render target 3
};


// ----------------------------------------------------------------------------
float4 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW, out float4 normalizedTangent)
{
	// 각 성분을 [0,1]에서 [-1,1]로 사상한다.
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// 정규직교 기저를 구축한다.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);
	
	// 접공간에서 세계 공간으로 변환한다.
	float4 bumpedNormalW = float4(mul(normalT, TBN), 1);
	//float3 bumpedNormalW = mul(normalMapSample, TBN);
	//float3 bumpedNormalW = normalMapSample;
	//float3 bumpedNormalW = normalT;

	//normalizedTangent = normalize(float4(T, 1));
	//normalizedTangent = normalize(float4(tangentW, 1));
	normalizedTangent = float4(mul(tangentW, TBN), 1);

	return bumpedNormalW;
}


VS_OUTPUT BasicVS(VS_INPUT vin)
{
	VS_OUTPUT output;

	// Transform to world space space.
	output.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	output.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	output.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	// Transform to homogeneous clip space.
	output.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	output.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//vout.Tex = vin.Tex;
	//output.Color = float4(output.NormalW.x, output.NormalW.y, output.NormalW.z, 1.0f);

	return output;
}


PS_OUTPUT BasicPS(VS_OUTPUT pin)
{
	PS_OUTPUT output;

	pin.NormalW = normalize(pin.NormalW);
	
	float3 diffuseAlbedo = gDiffuseMap.Sample(samAnisotropic, pin.Tex).xyz;

	float3 normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).xyz;
	
	float4 orthonormalizedTangent;
	float4 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW, orthonormalizedTangent);

	output.Position = float4(pin.PosW, 1.0f);
	output.Normal = float4(pin.NormalW.x, pin.NormalW.y, pin.NormalW.z, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.BumpedNormalW = bumpedNormalW;
	output.Tangent = orthonormalizedTangent;

	return output;
}


technique11 BasicTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, BasicPS()));
	}
}

