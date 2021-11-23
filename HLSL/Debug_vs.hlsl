#include "Debug.hlsli"

PSInput main( float4 position : POSITION )
{
	PSInput output;
	output.position = mul(position, world_view_projection);
	output.color = color;

	return output;
}