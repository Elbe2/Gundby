// ------------------------------------------------------------------
// Lukas Beinlich, 26.07.2014
// light.vs
// Ein einfacher Vertex Shader für Texturen mit diffusem Licht
// ------------------------------------------------------------------

cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

// Der eigentliche Shader
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float3 worldPosition;

	input.position.w = 1.0f;
	output.position = mul(input.position,WorldMatrix);
	output.position = mul(output.position,ViewMatrix);
	output.position = mul(output.position,ProjectionMatrix);
	
	output.tex = input.tex;
	
	output.normal = mul(input.normal,(float3x3)WorldMatrix);
	output.normal = normalize(output.normal);

	worldPosition=mul(input.position,WorldMatrix);
	output.viewDirection=cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection=normalize(output.viewDirection);

	return output;
}
