Texture2D ColorTexture : register(t0);
SamplerState LinearSampler : register(s0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
	float3 light : LIGHT;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float3 n = normalize(input.nor);
	float3 l = normalize(input.light);
	float4 illum = max(dot(l, n), 0) + 0.2f;
	float4 surfCol = float4(ColorTexture.Sample(LinearSampler, input.tex));
	
	return surfCol * illum;
}