/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		16/05/2012 												*/
/************************************************************************/

#include "Utility.fx"

Texture2D tex_gBuffer0;	//NORMAL + SPECULAR
Texture2D tex_gBuffer1;	//DIFFUSE + 
Texture2D tex_gBuffer2;	//DEPTH(only one channel)

Texture2D lightPass;
Texture2D SSAOPass;
Texture2D shadowMap;

//as we used a 0.01f scale when rendering to light buffer,
//revert it back here.
const static float LightBufferScaleInv = 1.0f;

cbuffer EveryFrame
{
	matrix	gViewProjectionInverse;
	matrix  lightVP;
	float4	cameraPosW;
	float SMAP_SIZE;
	float SHADOW_EPSILON;
	
};

struct VSInput
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PSSceneIn
{
	float2 Tex	: TEXCOORD1;
	float4 Pos	: SV_POSITION;
};

RasterizerState rasterState {
    FillMode = Solid;
	CullMode = Back;
};

SamplerState shadow_sample
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState sample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

PSSceneIn VS(VSInput input)
{
	PSSceneIn output;

	output.Tex = input.Tex;
	output.Pos = float4(input.Pos,1.0f);
	
	return output;
}
float doShadowing( float4 lightPositionH )
{
	lightPositionH.xy /= lightPositionH.w;
	
	float2 smTexCoord = float2(0.5f*lightPositionH.x, -0.5f*lightPositionH.y)+0.5f;
	
	if (smTexCoord.x < 0.0f || smTexCoord.x > 1.0f || smTexCoord.y < 0.0f || smTexCoord.y > 1.0f )
		return 1.0f;
	
	float depth = lightPositionH.z/lightPositionH.w;
	
	float dx = 1.0f/SMAP_SIZE;
	float s0 = (shadowMap.Sample(shadow_sample, smTexCoord).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s1 = (shadowMap.Sample(shadow_sample, smTexCoord + float2 (dx,0.0f)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s2 = (shadowMap.Sample(shadow_sample, smTexCoord + float2 (0.0f,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s3 = (shadowMap.Sample(shadow_sample, smTexCoord + float2 (dx,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	
	float2 texelPos = smTexCoord * SMAP_SIZE;
	float2 lerps = frac(texelPos);
	
	return float (lerp(lerp(s0,s1,lerps.x),lerp(s2,s3,lerps.x),lerps.y));
}
float4 PS(PSSceneIn input) : SV_TARGET
{
	float4 normal		= tex_gBuffer0.Sample(sample, input.Tex);
	float3 diffuse		= tex_gBuffer1.Sample(sample, input.Tex).rgb;
	float depthValue	= tex_gBuffer2.Sample(sample, input.Tex).r;
	float3 lightColor	= lightPass.Sample(sample, input.Tex).rgb * LightBufferScaleInv;
	float  SSAO			= SSAOPass.Sample(sample, input.Tex).r;		
	
	float3 worldPos = GetWorldPosition(input.Tex,depthValue,gViewProjectionInverse);
	if ( depthValue > 0.9999f )
	{	
		return float4 (worldPos.xyz,1.0f);
	}
	float4 PosLightH = mul( float4 (worldPos,1.0f), lightVP);
	float shadowCoeff = doShadowing(PosLightH);
	
	float4 finalColor = float4 (((diffuse*shadowCoeff) - SSAO)* lightColor,1);
	finalColor.rgb += diffuse*0.1f;
	/*
	float4 finalColor = float4(diffuse.xyz,1.0f);
	finalColor.xyz *=shadowCoeff;
	finalColor = float4 (finalColor.r-SSAO,finalColor.g-SSAO,finalColor.b-SSAO,1.0f);
	finalColor.xyz += diffuse.xyz*(lightColor.xyz*0.01f);
	finalColor.xy += diffuse.xy*0.1f;
	finalColor.z += diffuse.z*0.2f;
	*/
	//return float4 (s0,s0,s0,1.0f);
	//return float4 (depthValue,depthValue,depthValue,1.0f);
	return finalColor;
	//return float4 (normal.r,normal.g,normal.b,1.0f);
}

technique10 Technique_0
{
	pass Pass_0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
		SetRasterizerState( rasterState );
	}
}