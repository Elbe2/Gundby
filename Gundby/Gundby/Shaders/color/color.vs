// ------------------------------------------------------------------
// Lukas Beinlich, 24.07.2014
// color.vs
// Ein sehr einfacher Vertex Shader
// ------------------------------------------------------------------

cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// Der eigentlich Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position,WorldMatrix);
	output.position = mul(output.position,ViewMatrix);
	output.position = mul(output.position,ProjectionMatrix);
	output.color = input.color;
	return output;
}
