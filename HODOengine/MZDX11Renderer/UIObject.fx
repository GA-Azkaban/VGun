
cbuffer cbPerObject
{
	float4x4 gWorld;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = MIN_MAG_MIP_LINEAR;

	MaxAnisotropy = 4;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//vout.PosH = float4(vin.PosL, 1.0f);
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.Tex = vin.Tex;

	return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
	float4 texColor = float4(1, 1, 1, 1);
	texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);

	return texColor;
}

technique11 DrawUITech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
