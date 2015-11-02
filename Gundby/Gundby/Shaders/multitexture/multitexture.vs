// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// multitexture.vs
// Ein Vertex Shader für viele Texturen und Texturearrays
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
PixelInputType MultiTextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position,WorldMatrix);
	output.position = mul(output.position,ViewMatrix);
	output.position = mul(output.position,ProjectionMatrix);
	output.tex = input.tex;
	return output;
}
