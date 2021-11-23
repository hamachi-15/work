#include "UseCubeMap.hlsli"
float4 main(PSInput input) : SV_Target
{
	float4 color = diffuse_texture.Sample(decal_sampler,input.texcoord);
	float3 E = normalize(input.wposition - eye_pos);
	float3 N = normalize(input.wnormal);
	//”½Ë
	float3 R1 = (float3)0;
	R1 = reflect(E, N);

	//‹üÜE“§‰ß
	float3 R2 = (float3)0;
	R2 = refract(E, N, 1.0 / 1.5);  //
	//ƒtƒŒƒlƒ‹‚Û‚¢‚à‚Ì
	float ratio = pow(dot(-E, N),2);
	color.rgb += cube_texture.Sample(decal_sampler,R1).rgb * (1.0 - ratio);
	color.rgb += cube_texture.Sample(decal_sampler,R2).rgb * ratio;

	return color;
}