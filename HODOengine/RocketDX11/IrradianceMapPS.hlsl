#include "Sampler.hlsli"

TextureCube CubeMap : register(t0);

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float3 uv	: TEXCOORD;
};

static const float PI = 3.14159265359;
static const float SAMPLE_DELTA = 0.05;

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 irradiance = float3(0.0f, 0.0f, 0.0f);
	float3 normal = normalize(input.uv);

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = cross(up, normal);
	up = cross(normal, right);

	float nrSamples = 0.0f;
	for (float phi = 0.0f; phi < 2.0 * PI; phi += SAMPLE_DELTA)
	{
		for (float theta = 0.0f; theta < 0.5 * PI; theta += SAMPLE_DELTA)
		{
			// spherical to cartesian (in tangent space)
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
			irradiance += CubeMap.Sample(LinearWrapSampler, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    irradiance.x *= 1.05f;
    irradiance.y *= 0.965f;
    irradiance.z *= 1.045f;
	
	return float4(irradiance, 1.0f);
}