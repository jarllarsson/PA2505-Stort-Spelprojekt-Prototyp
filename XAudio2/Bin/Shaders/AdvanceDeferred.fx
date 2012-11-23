/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/9/2012 												*/
/************************************************************************/
cbuffer EveryFrame
{
	matrix gWVP;						//World View Projection
	matrix gWorld;
};

Texture2D gAlbedoMap;
Texture2D gNormalMap;
Texture2D gSpecularMap;

SamplerState gTriLinearSample
{
	Filter = MIN_MAG_MIP_LINEAR;
};

//-----------------------------------------------------------------------------------------
// Vertex shader input structure
//-----------------------------------------------------------------------------------------
struct VSIn
{
	float3 PosLocal	: POSITION;
	float2 TexCord	: TEXCORD;
	float3 NorLocal	: NORMAL;
};

//-----------------------------------------------------------------------------------------
// Vertex shader output
//-----------------------------------------------------------------------------------------
struct PSSceneIn
{
	float4 PosH		: SV_POSITION;		// vertex screen coordinates
	float3 NormW	: NORMAL;
	float2 TexCord	: TEXCORD;
};

//-----------------------------------------------------------------------------------------
// Pixel shader output, write to diffuse and albedo
//-----------------------------------------------------------------------------------------
struct PSOut
{
	float4 normal	: SV_TARGET0;		//NORMAL + SPECULAR
	float4 albedo	: SV_TARGET1;		//ALBEDO
};

PSSceneIn VS( VSIn input )
{
	PSSceneIn output	= (PSSceneIn)0;
	output.TexCord		= input.TexCord;
	output.PosH		=	mul (float4(input.PosLocal,1), gWVP);
	output.NormW 	= 	mul (float4(input.NorLocal,0), gWorld).xyz;
	
	return output;							
}


PSOut PS( PSSceneIn input )
{
	PSOut output	= (PSOut)0;

	output.albedo = gAlbedoMap.Sample(gTriLinearSample, input.TexCord);
	float specular = gSpecularMap.Sample(gTriLinearSample, input.TexCord).r;
	float3 sampleNormal = gNormalMap.Sample(gTriLinearSample, input.TexCord).rgb;
	sampleNormal = 2.0f*sampleNormal - 1.0f;
	
	float3 normal = input.NormW-sampleNormal;
	
	normal = (normal + 1.0f)*0.5f;
	//normal = normal / 2.0f;

	output.normal = float4 (normal,0.0f);
	output.normal.w = specular;

	return output;
}


// the primary technique
technique10 Technique_0
{
	// the primary pass
	pass Pass_0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}