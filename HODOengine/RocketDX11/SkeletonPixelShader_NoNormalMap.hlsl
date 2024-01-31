
struct DirectionalLight
{
	float4 Color;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float4 Position;
};

struct SpotLight
{
	float4 Color;
	float4 Position;
	float3 Direction;
	float SpotPower;
};

cbuffer lightData : register(b0)
{
	DirectionalLight dirLight;
	PointLight pointLight[4];
	SpotLight spotLight[2];

	float3 cameraPosition;
}

// External texture-related data
Texture2D Texture		: register(t0);
SamplerState Sampler	: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Sample the texture
	float4 textureColor = Texture.Sample(Sampler, input.uv);

	float3 toCamera = normalize(cameraPosition - input.worldPos);

	// Directional light calculation
	float dirLightAmount = saturate(dot(input.normal, -normalize(dirLight.Direction)));
	float3 totalDirLight = dirLight.Color * dirLightAmount * textureColor;

	// Point light calculation
	float3 totalPointLight = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		float dirToPointLight = normalize(pointLight[i].Position - input.worldPos);
		float pointLightAmount = saturate(dot(input.normal, dirToPointLight));
		float3 refl = reflect(-dirToPointLight, input.normal);
		//float spec = pow(max(dot(refl, toCamera), 0), 32);
		//totalPointLight += pointLight[i].Color * pointLightAmount * textureColor + spec;
		totalPointLight += pointLight[i].Color * pointLightAmount * textureColor;
	}

	// Spot light calculation
	float3 totalSpotLight = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 2; ++i)
	{
		float dirToSpotLight = normalize(spotLight[i].Position - input.worldPos);
		float angleFromCenter = max(dot(dirToSpotLight, spotLight[i].Direction), 0.0f);
		float spotAmount = pow(angleFromCenter, spotLight[i].SpotPower);
		totalSpotLight += (spotAmount * spotLight[i].Color * textureColor);
	}

	float3 totalLight = totalDirLight + totalPointLight + totalSpotLight;

	return float4(totalLight, 1.0f);
}