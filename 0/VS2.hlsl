cbuffer CBCamera : register(b0)
{
	matrix viewProj;
};
cbuffer CBWorld : register(b1)
{
	matrix world;
};
cbuffer CBLight : register(b2)
{
	float3 lightPosW;
	float4 lightCol;
	float pad;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
	float3 light : LIGHT;
};

VS_OUTPUT main(float3 pos : POSITION, float3 nor : NORMAL, float2 tex : TEXCOORD)
{
	VS_OUTPUT o;
	o.pos = mul(float4(pos, 1.0f), world);
	float3 posW = o.pos.xyz;
	o.pos = mul(o.pos, viewProj);

	o.nor = mul(nor, (float3x3) world);
	o.tex = tex;
	o.light = lightPosW;// -posW;

	return o;
}


