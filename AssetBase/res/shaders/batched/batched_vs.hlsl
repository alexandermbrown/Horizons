#include "ViewProjCB.h"

struct VERTEX_IN
{
	float3 position : POSITION;
	float texindex : TEXINDEX;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	int texindex : TEXINDEX;
};

PS_IN vs_main(VERTEX_IN input)
{
	PS_IN output;

	output.position = mul(u_ViewProj, float4(input.position, 1.0));

	output.texcoord = input.texcoord;
	output.color = input.color;
	output.texindex = int(input.texindex);

	return output;
}
