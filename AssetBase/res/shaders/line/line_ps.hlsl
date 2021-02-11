
struct PS_IN
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 ps_main(PS_IN input) : SV_TARGET
{
	return input.color;
}
