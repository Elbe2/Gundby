// ------------------------------------------------------------------
// colorvs.hlsl
// Lukas Beinlich, 05.10.2015
// Ein einfacher Vertexshader
// ------------------------------------------------------------------

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return input.color;
}