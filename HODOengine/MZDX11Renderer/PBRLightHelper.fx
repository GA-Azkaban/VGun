
static const float PI = 3.141592;
static const float Epsilon = 0.00001;

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Auxiliary methods for calculating the lightings
float NormalDistributionGGXTR(float3 normal, float3 halfwayVec, float3 roughness)
{
	float roughnessSquare = roughness * roughness;
	float NdotH = saturate(dot(normal, halfwayVec));
	float NdotHSquare = NdotH * NdotH;

	float nom = roughnessSquare;
	float denom = (NdotHSquare * (nom - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f;

	float nom = NdotV;
	float denom = nom * (1.0f - k) + k;

	return nom / denom;
}

float GeometrySmith(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
	// Geometry Obstruction
	float NdotV = saturate(dot(normal, viewDir));

	// Geometry Shadowing
	float NdotL = saturate(dot(normal, lightDir));

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 R0)
{
	// R0은 반사율
	return (R0 + (1.0f - R0) * pow(1.0f - cosTheta, 5.0f));
}

//---------------------------------------------------------------------------------------

void ComputeDirectionalLight(DirectionalLight L,
	float3 normal, float3 diffuseAlbedo, 
	float3 viewDir, out float3 Rad)
{
	float metallic = 0.5f;
	float roughness = 0.3f;

	// Initialize outputs.
	Rad = float3(0.0f, 0.0f, 0.0f);

	float gamma = 2.2f;
	diffuseAlbedo = pow(diffuseAlbedo, gamma);	// linear space

	float3 radiance = L.Diffuse;

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -normalize(L.Direction);
	float3 halfwayVec = normalize(viewDir + lightVec);

	float3 F0 = 0.04f; // 초기값이 0.04f인 이유는 0.04가 금속재질이 아닌 경우에 가장 적합하다고 알려져있기 때문이다.
	F0 = lerp(F0, diffuseAlbedo, metallic);

	// Cook-Torrance specular BRDF
	float D = NormalDistributionGGXTR(normal, halfwayVec, roughness);
	float G = GeometrySmith(normal, viewDir, lightVec, roughness);
	float3 F = FresnelSchlick(max(dot(halfwayVec, viewDir), 0.0f), F0);
	float3 nominator = D * G * F;

	//Wo = View Direction
	//Wi = Light Direction
	float WoDotN = saturate(dot(viewDir, normal));
	float WiDotN = saturate(dot(L.Direction, normal));
	float denominator = (4 * WoDotN * WiDotN);

	float specular = nominator / (denominator + 0.001f);

	// Energy Conservation
	float3 kS = F;	// reflection energy
	float3 kD = 1.0f - kS;	// refraction energy
	kD *= 1.0f - metallic;

	Rad = (((kD * diffuseAlbedo / PI) + specular) * radiance * WiDotN);
}


void ComputePointLight(PointLight L, float3 pos, float3 normal, 
	float3 diffuseAlbedo, float3 viewDir, out float3 Rad)
{
	float metallic = 0.5f;
	float roughness = 0.3f;

	// Initialize outputs.
	Rad = float3(0.0f, 0.0f, 0.0f);

	float gamma = 2.2f;
	diffuseAlbedo = pow(diffuseAlbedo, gamma);	// linear space

	float3 Lo = -viewDir; // outgoing light direction(vector from world-space fragment position to the eye)
	float cosLo = max(0.0, dot(normal, Lo)); // Angle between surface normal and outgoing light direction
	float distance = length(L.Position - pos);

	// Range test
	if (distance > L.Range)
		return;

	float attenuation = 10.0f / (distance * distance);
	float3 radiance = L.Diffuse * attenuation * cosLo;

	// The light vector aims opposite the direction the light rays travel.
	//float3 lightVec = -L.Direction;
	float3 lightVec = normalize(L.Position - pos);
	float3 halfwayVec = normalize(viewDir + lightVec);

	float3 F0 = 0.04f; // 초기값이 0.04f인 이유는 0.04가 금속재질이 아닌 경우에 가장 적합하다고 알려져있기 때문이다.
	F0 = lerp(F0, diffuseAlbedo, metallic);

	// Cook-Torrance specular BRDF
	float D = NormalDistributionGGXTR(normal, halfwayVec, roughness);
	float G = GeometrySmith(normal, viewDir, lightVec, roughness);
	float3 F = FresnelSchlick(max(dot(halfwayVec, viewDir), 0.0f), F0);
	float3 nominator = D * G * F;

	//Wo = View Direction
	//Wi = Light Direction
	float WoDotN = saturate(dot(viewDir, normal));
	//float WiDotN = saturate(dot(L.Direction, normal));
	float WiDotN = saturate(dot(-lightVec, normal));
	float denominator = (4 * WoDotN * WiDotN);

	float specular = nominator / (denominator + 0.001f);

	// Energy Conservation
	float3 kS = F;	// reflection energy
	float3 kD = 1.0f - kS;	// refraction energy
	kD *= 1.0f - metallic;

	Rad = (((kD * diffuseAlbedo / PI) + specular) * radiance * WiDotN);
}


void ComputeSpotLight(SpotLight L, float3 pos, float3 normal, 
	float3 diffuseAlbedo, float3 viewDir, out float3 Rad)
{
	float metallic = 0.5f;
	float roughness = 0.3f;

	// Initialize outputs.
	Rad = float3(0.0f, 0.0f, 0.0f);

	float gamma = 2.2f;
	diffuseAlbedo = pow(diffuseAlbedo, gamma);	// linear space

	float3 Lo = -viewDir; // outgoing light direction(vector from world-space fragment position to the eye)
	float cosLo = max(0.0, dot(normal, Lo)); // Angle between surface normal and outgoing light direction
	float distance = length(L.Position - pos);

	// Range test
	if (distance > L.Range)
		return;

	float3 lightVec = normalize(L.Position - pos);
	float3 halfwayVec = normalize(viewDir + lightVec);

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
	float attenuation = spot * 10.0f / (distance * distance);
	float3 radiance = L.Diffuse * attenuation * cosLo;

	float3 F0 = 0.04f; // 초기값이 0.04f인 이유는 0.04가 금속재질이 아닌 경우에 가장 적합하다고 알려져있기 때문이다.
	F0 = lerp(F0, diffuseAlbedo, metallic);

	// Cook-Torrance specular BRDF
	float D = NormalDistributionGGXTR(normal, halfwayVec, roughness);
	float G = GeometrySmith(normal, viewDir, lightVec, roughness);
	float3 F = FresnelSchlick(max(dot(halfwayVec, viewDir), 0.0f), F0);
	float3 nominator = D * G * F;

	//Wo = View Direction
	//Wi = Light Direction
	float WoDotN = saturate(dot(viewDir, normal));
	float WiDotN = saturate(dot(L.Direction, normal));
	float denominator = (4 * WoDotN * WiDotN);

	float specular = nominator / (denominator + 0.001f);

	// Energy Conservation
	float3 kS = F;	// reflection energy
	float3 kD = 1.0f - kS;	// refraction energy
	kD *= 1.0f - metallic;

	Rad = (((kD * diffuseAlbedo / PI) + specular) * radiance * WiDotN);
}
