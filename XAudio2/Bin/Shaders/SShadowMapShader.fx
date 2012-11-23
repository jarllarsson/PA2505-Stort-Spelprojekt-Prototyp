/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		22/10/2012 												*/
/************************************************************************/

cbuffer PerObject
{
	matrix gLightWVP;
};

struct VSIn
{
	float3 PosLocal	: POSITION;
	float2 TexCord	: TEXCORD;
	float3 NorLocal	: NORMAL;
};

float4 VS(VSIn input): SV_POSITION
{
	return mul(float4 (input.PosLocal,1.0f), gLightWVP);
}

technique10 RenderShadowMap
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( NULL );
	}
}