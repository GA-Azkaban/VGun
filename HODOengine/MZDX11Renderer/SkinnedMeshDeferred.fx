#include "LightHelper.fx"


cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[96];
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
	float3 PosL			: POSITION;
	float3 NormalL		: NORMAL;
	float2 Tex			: TEXCOORD;
	float3 TangentL		: TANGENT;
	float Weights0 : BLENDWEIGHT0;
	float Weights1 : BLENDWEIGHT1;
	float Weights2 : BLENDWEIGHT2;
	uint BoneIndices0 : BLENDINDICES0;
	uint BoneIndices1 : BLENDINDICES1;
	uint BoneIndices2 : BLENDINDICES2;
	uint BoneIndices3 : BLENDINDICES3;
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
	float3 BumpedNormalW	: SV_Target3;
	//float4 ZBuffer		: SV_Target3; // Render target 3
	float4 Tangent			: SV_Target4; // Render target 3
};

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


VS_OUTPUT BasicVS(VS_INPUT vin)
{
	VS_OUTPUT output;

	// 배열을 초기화한다. 그렇지 않으면 SV_POSITION에 관련된 경고 메시지가 나온다.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vin.Weights0;
	weights[1] = vin.Weights1;
	weights[2] = vin.Weights2;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	uint boneIndices[4] = { 0, 0, 0, 0 };
	boneIndices[0] = vin.BoneIndices0;
	boneIndices[1] = vin.BoneIndices1;
	boneIndices[2] = vin.BoneIndices2;
	boneIndices[3] = vin.BoneIndices3;

	// 정점 혼합을 수행한다.
	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		// 법선 변환 시 변환행렬에 비균등 비례가 없다고 가정하고
		// 변환 행렬을 그대로 사용한다. (역전치행렬이 아니라)	
		posL += weights[i] * mul(float4(vin.PosL, 1.0f),
			gBoneTransforms[boneIndices[i]]).xyz;
		normalL += weights[i] * mul(vin.NormalL,
			(float3x3)gBoneTransforms[boneIndices[i]]);
		tangentL += weights[i] * mul(vin.TangentL.xyz,
			(float3x3)gBoneTransforms[boneIndices[i]]);

	}

	// Transform to world space space.
	output.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
	output.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);

	output.TangentW = mul(tangentL, (float3x3)gWorld);

	// Transform to homogeneous clip space.
	output.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
	output.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//vout.Tex = vin.Tex;
	//output.Color = float4(output.NormalW.x, output.NormalW.y, output.NormalW.z, 1.0f);

	return output;
}

// ----------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW, out float4 normalizedTangent)
{
	// 각 성분을 [0,1]에서 [-1,1]로 사상한다.
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// 정규직교 기저를 구축한다.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// 접공간에서 세계 공간으로 변환한다.
	float3 bumpedNormalW = mul(normalT, TBN);

	normalizedTangent = normalize(float4(T, 1));

	return bumpedNormalW;
}

PS_OUTPUT BasicPS(VS_OUTPUT pin)
{
	PS_OUTPUT output;

	pin.NormalW = normalize(pin.NormalW);

	float3 diffuseAlbedo = gDiffuseMap.Sample(samAnisotropic, pin.Tex).xyz;

	float3 normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).xyz;

	float4 orthonormalizedTangent;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW, orthonormalizedTangent);

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

