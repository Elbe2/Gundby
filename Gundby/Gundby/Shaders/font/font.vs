// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// font.vs
// Ein sehr einfacher Vertex Shader für die Schrift
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
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Der eigentlich Shader
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position,WorldMatrix);
	output.position = mul(output.position,ViewMatrix);
	output.position = mul(output.position,ProjectionMatrix);
	output.tex = input.tex;
	return output;
}
