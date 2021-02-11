
struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

cbuffer Font : register(b2)
{
	float4 u_Color;
	float u_DistanceFactor;
};

Texture2D u_Texture : register( t0 );
SamplerState u_Sampler : register( s0 );

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	float3 colorsample = u_Texture.Sample(u_Sampler, input.texcoord).rgb;
	float sigDist = u_DistanceFactor * (median(colorsample.r, colorsample.g, colorsample.b) - 0.5);
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	return float4(u_Color.rgb, opacity * u_Color.a);
}
