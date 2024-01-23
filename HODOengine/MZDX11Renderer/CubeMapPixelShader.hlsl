
TextureCube Texture	: register(t0);
SamplerState Sampler	: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
};

struct PSOutput
{
	float4 position		: SV_Target0;
	float4 diffuse		: SV_Target1;
	float4 normal		: SV_Target2;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return Texture.Sample(Sampler, input.worldPos);
}

//PSOutput main(VertexToPixel input)
//{
//	PSOutput output;
//
//	// Sample the texture
//	float4 textureColor = Texture.Sample(Sampler, input.worldPos);
//
//	output.position = float4(input.worldPos, 1.0f);
//	output.diffuse = textureColor;
//	output.normal = float4(normalize(input.normal), 1.0f);
//
//	return output;
//}
