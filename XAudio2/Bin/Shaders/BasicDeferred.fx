cbuffer EveryFrame
{
	matrix gWVP;						//World View Projection
	matrix gWorld;
	matrix texMat;
};

Texture2D gAlbedoMap;

//-----------------------------------------------------------------------------------------
// Sampler Structures
//-----------------------------------------------------------------------------------------
SamplerState gTriLinearSample
{
	Filter		= ANISOTROPIC;
	AddressU	= Wrap;
	AddressV	= Wrap;
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
// Pixel shader input
//-----------------------------------------------------------------------------------------
struct PSSceneIn
{
	float4 PosH		: SV_POSITION;			// vertex screen coordinates
	float3 NorW		: NORMAL;
	float2 TexCord	: TEXCORD;				// vertex color				
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
	output.TexCord 		= float2(mul(float4(input.TexCord, 0.0f, 1.0f), texMat).xy);

	output.PosH = mul (float4 (input.PosLocal,1), gWVP);	
	float4 normW = mul (float4 (input.NorLocal,0), gWorld);
	normW.xyz = normalize(normW.xyz);
	output.NorW = normW.xyz;
	return output;						
}


PSOut PS( PSSceneIn input )
{
	PSOut output = (PSOut)0;

	float3 normal = input.NorW + 1.0f;
	normal = normal/2.0f;

	output.albedo = gAlbedoMap.Sample(gTriLinearSample, input.TexCord);
	output.normal = float4(normal,0.0f);
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