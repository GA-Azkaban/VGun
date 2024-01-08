
struct DirectionalLight
{
	float4 Color;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float3 Position;
};

struct SpotLight
{
	float4 Color;
	float3 Position;
	float3 Direction;
	float SpotPower;
};

cbuffer lightData : register(b0)
{
	DirectionalLight dirLight;
	PointLight pointLight[5];
	SpotLight spotLight[5];

	float3 cameraPosition;
}

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
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

	// Read and unpack normal from map
	float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).xyz * 2 - 1;

	// Transform from tangent to world space
	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);

	float3x3 TBN = float3x3(T, B, N);
	input.normal = normalize(mul(normalFromMap, TBN));

	// Sample the texture
	float4 textureColor = Texture.Sample(Sampler, input.uv);

	float3 toCamera = normalize(cameraPosition - input.worldPos);

	// Directional light calculation
	float dirLightAmount = saturate(dot(input.normal, -normalize(dirLight.Direction)));
	float totalDirLight = dirLight.Color * dirLightAmount * textureColor;

	// Point light calculation
	float totalPointLight = 0.0f;
	for (int i = 0; i < 4; ++i)
	{
		float dirToPointLight = normalize(pointLight[i].Position - input.worldPos);
		float pointLightAmount = saturate(dot(input.normal, dirToPointLight));
		float3 refl = reflect(-dirToPointLight, input.normal);
		float spec = pow(max(dot(refl, toCamera), 0), 32);
		totalPointLight += pointLight[i].Color * pointLightAmount * textureColor + spec;
	}

	// Spot light calculation
	float totalSpotLight = 0.0f;
	for (int i = 0; i < 2; ++i)
	{
		float dirToSpotLight = normalize(spotLight[i].Position - input.worldPos);
		float angleFromCenter = max(dot(dirToSpotLight, spotLight[i].Direction), 0.0f);
		float spotAmount = pow(angleFromCenter, spotLight[i].SpotPower);
		totalSpotLight += (spotAmount * spotLight[i].Color * textureColor);
	}

	float3 totalLight = totalDirLight + totalPointLight + totalSpotLight;

	//return float4(totalLight, 1.0f);
	return textureColor;
}