
struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

Texture2D u_Texture : register( t0 );
SamplerState u_Sampler : register( s0 );

float4 ps_main(PS_IN input) : SV_TARGET
{
	return u_Texture.Sample(u_Sampler, input.texcoord);
}
