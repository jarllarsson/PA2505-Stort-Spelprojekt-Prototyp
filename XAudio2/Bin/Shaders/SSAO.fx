/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		18/10/2012 												*/
/************************************************************************/

//-----------------------------------------------------------------------------------------
// !!!The UV coordinate is not the same as the texture coordinate!!!
//-----------------------------------------------------------------------------------------

#include "Utility.fx"

Texture2D tex_normalBuffer;
Texture2D tex_randNormals;
Texture2D tex_depthBuffer;

cbuffer EveryFrame
{
	matrix	gInvVP;
	matrix 	gWVP;
	float2 	gScreenSize;
	float 	gRandomTexSize; //It's the size of the random normals texture
	float  	gScale;
	float	gBias;
	float 	gIntensity;
	float 	gSampleRadius;
};

SamplerState samp_normal
{
	Filter = MIN_MAG_MIP_LINEAR;
};
SamplerState samp_rand
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState samp_depth
{
	Filter = MIN_MAG_MIP_POINT;
};

//-----------------------------------------------------------------------------------------
// Vertex shader input structure
//-----------------------------------------------------------------------------------------
struct VSInput
{
	float3 Pos		: POSITION;
	float2 TexCoord	: TEXCOORD0;
};
//-----------------------------------------------------------------------------------------
// Pixel shader input
//-----------------------------------------------------------------------------------------
struct PSInput
{
	float2 TexCoord		: TEXCOORD0;
	float4 Pos			: SV_POSITION;
};

//-----------------------------------------------------------------------------------------
// Get the world position from the UV coordinate
//-----------------------------------------------------------------------------------------
float3 getPosition( float2 uv)
{
	float depthValue = tex_depthBuffer.Sample(samp_depth, uv).r;
		
	return GetWorldPosition( uv, depthValue, gInvVP);
}
//-----------------------------------------------------------------------------------------
// Gets the normal stored in the normalbuffer
//-----------------------------------------------------------------------------------------
float3 getNormal( float2 uv )
{
	return tex_normalBuffer.Sample(samp_normal,uv).xyz*2.0f - 1.0f;
}
//-----------------------------------------------------------------------------------------
// Gets a sampled random normal from the random normal buffer
//-----------------------------------------------------------------------------------------
float2 getRandom( float2 uv )
{
	return normalize(tex_randNormals.Sample(samp_rand, gScreenSize*uv / gRandomTexSize).xy * 2.0f - 1.0f);
}
float doAmbientOcclusion( float2 tcoord, float2 uv, float3 position, float3 normal)
{
	float3 diff = getPosition(tcoord + uv) - position;
	float3 v = normalize(diff);
	float d = length(diff)*gScale;
	return max(0.0f,dot(normal,v)-gBias)*(1.0f/(1.0f+d))*gIntensity;
}

PSInput VS( VSInput input )
{
	PSInput data 	= (PSInput)0;
	data.TexCoord 	= input.TexCoord;
	data.Pos		= float4(input.Pos,1.0f); 
	
	return data;
}

float PS( PSInput input ) : SV_TARGET
{	
	const float2 vec[4] = { float2 (1,0), float2 (-1,0),
							float2 (0,1), float2 (0,-1)};
	
	float depthValue = tex_depthBuffer.Sample(samp_depth, input.TexCoord).r;
	
	if (depthValue > 0.999f)
		clip(-1);
	
	float3 position = getPosition(input.TexCoord);
	float3 normal	= getNormal(input.TexCoord);
	float2 rand		= getRandom(input.TexCoord);
	
	float ao 		= 0.0f;
	float radius	= gSampleRadius/position.z;
	
	const int iterations = 4;
	for ( int i = 0; i < iterations; i++)
	{
		float2 coord1 = reflect(vec[i], rand)*radius;
		float2 coord2 = float2 (coord1.x*0.707f - coord1.y*0.707f,
								coord1.x*0.707f + coord1.y*0.707f);
								
		ao += doAmbientOcclusion(input.TexCoord, coord1*0.25f, position, normal);
		ao += doAmbientOcclusion(input.TexCoord, coord2*0.5f, position, normal);
		ao += doAmbientOcclusion(input.TexCoord, coord1*0.75f, position, normal);
		ao += doAmbientOcclusion(input.TexCoord, coord2, position, normal);
	}
	
	ao/=(float)iterations*4.0f;
	
	return ao;
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