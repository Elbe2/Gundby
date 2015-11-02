// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// font.vs
// Ein sehr Vertex Shader für die Schrift
// ------------------------------------------------------------------

cbuffer PositionArrayBuffer
{
	float4 Pos[2048];
};

cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VertexInputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// Der eigentlich Shader
PixelInputType FontVertexShader(uint instanceID : SV_InstanceID,uint vertexID : SV_VertexID,VertexInputType input)
{
	PixelInputType output;

	uint nr = vertexID + instanceID * 4;

	output.position.x = Pos [nr].x;
	output.position.y = Pos [nr].y;
	
	output.position.z = 0.0f;
	output.position.w = 1.0f;

	output.position = mul(output.position,world);
	output.position = mul(output.position,view);
	output.position = mul(output.position,proj);
	

	output.tex.x = Pos[nr].z;
	output.tex.y = Pos[nr].w;
	return output;
}
