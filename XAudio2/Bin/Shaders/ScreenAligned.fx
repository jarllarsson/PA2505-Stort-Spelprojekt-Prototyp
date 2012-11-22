/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		18/10/2012 												*/
/************************************************************************/

Texture2D tex_sampleTexture;

cbuffer PerObject{
	float4 activeChannels;
	float negateChannelSamp; // 0 == don't do anything
};

SamplerState samp_tex
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
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

PSInput VS( VSInput input )
{
	PSInput data = (PSInput)0;
	data.TexCoord 	= input.TexCoord;
	data.Pos		= float4(input.Pos,1.0f); 
	
	return data;
}

float4 PS( PSInput input ) : SV_TARGET
{
	float4 finalColor = float4(0,0,0,1.0f);
	float wightOfActiveChannels= activeChannels.x + activeChannels.y + activeChannels.z + activeChannels.a;
	if(activeChannels.x == 1)
	{
		finalColor.x = tex_sampleTexture.Sample(samp_tex,input.TexCoord).x;
		if (negateChannelSamp)
			finalColor.x = 1-finalColor.x;
		if (wightOfActiveChannels == 1)
			return float4 (finalColor.x,finalColor.x,finalColor.x,1.0f);
	}
	if(activeChannels.y == 1)
		finalColor.y = tex_sampleTexture.Sample(samp_tex,input.TexCoord).y;
	if(activeChannels.z == 1)
		finalColor.z = tex_sampleTexture.Sample(samp_tex,input.TexCoord).z;
	if(activeChannels.a == 1)
		finalColor.a = tex_sampleTexture.Sample(samp_tex,input.TexCoord).a;
	

	return finalColor;
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