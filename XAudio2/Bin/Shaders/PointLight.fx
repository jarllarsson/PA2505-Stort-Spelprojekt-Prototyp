/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/18/2012 												*/
/************************************************************************/

#include "Utility.fx"

Texture2D tex_gBuffer0;	//NORMAL
Texture2D tex_gBuffer2;	//DEPTH

//we use this to avoid clamping our results into [0..1]. 
//this way, we can fake a [0..100] range, since we are using a
//floating point buffer
const static float LightBufferScale = 1.0f;

cbuffer EveryFrame
{
	matrix		gWVP;
	matrix		gInvVP;		//Inverse-view-projection-matrix
	float4		lightColor;
	float		range;
	float4		lightCenter;
	float3		lightAtt;	//Not used

	float2		SCREENSIZE;
};
struct VSIn
{
	float3 pos : POSITION;
};

struct PSIn
{
	float4 posH : SV_POSITION; 
};

SamplerState sampleState
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float CalculateLinearAttenuation(float3 lightVector, float range)
{
	float attuenation = saturate(1.0f - length(lightVector)/range);

	return attuenation;
}

PSIn VS( VSIn input )
{
	PSIn output;
	output.posH = mul(float4(input.pos,1.0f),gWVP);
	return output;
}

float4 PS(PSIn input) : SV_TARGET
{
	float2 textureCoord = float2(input.posH.x/SCREENSIZE.x,input.posH.y/SCREENSIZE.y);

	//Get the given pixels depth
	float depthValue = tex_gBuffer2.Sample(sampleState, textureCoord).r;
	
	//Discard any calculations on this pixel if the value is lower than zero
	clip(-depthValue + 0.999f);

	//Reconstruct the surfacepos from the sampled depth value 
	float3 surfacePosW = GetWorldPosition( textureCoord, depthValue, gInvVP);

	//Light direction from the current pixel to current light
	float3 lightDir = lightCenter.xyz - surfacePosW;

	//Compute the attenuation
	float attenuation = CalculateLinearAttenuation(lightDir,range)/2;

	//Get the normal
	float4 normalMap = tex_gBuffer0.Sample(sampleState, textureCoord);
	float3 normal = ConvertSampledNormal(normalMap);
	
	lightDir = normalize(lightDir);

	//N dot L lightning term, attenuated
	float nl = saturate(dot(normal,lightDir))*attenuation;

	//As our position is relative to camera position, we dont need to use ViewPosition - pos here
	float3 camDir = normalize(surfacePosW);

	//Calculate specular term;
	float3 h			= normalize(reflect(lightDir,normal));
	float spec			= nl * pow(saturate(dot(camDir,h)),normalMap.w*100);
	float4 finalColor	= float4(lightColor.xyz * nl, 0.5f);

	return finalColor*LightBufferScale;
}
technique10 Technique_0
{
	pass Pass_0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}