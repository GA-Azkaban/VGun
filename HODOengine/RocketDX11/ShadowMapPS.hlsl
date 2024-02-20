
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
	float depthValue = input.position.z / input.position.w;
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float4 color = float4(depthValue, depthValue, depthValue, 1.0f);
	return color;
}