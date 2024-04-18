
struct VertexToPixel
{
	float4 position : SV_POSITION;
};

// The entry point for our vertex shader
VertexToPixel main(uint id : SV_VertexID)
{
	VertexToPixel output;

	// calculate the UV(0,0) to (2,2) via the ID
	float2 uv = float2((id << 1) & 2, // id % 2 * 2
		id & 2);

	output.position = float4(uv, 0, 1);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * -2 + 1;

	return output;
}