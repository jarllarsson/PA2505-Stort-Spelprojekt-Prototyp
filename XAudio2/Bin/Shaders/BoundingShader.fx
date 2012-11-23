/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		8/10/2012 												*/
/************************************************************************/
cbuffer EveryFrame
{
	matrix gWVP;						//World View Projection
	float4 color;
};

Texture2D gAlbedoMap;

RasterizerState rasterState {
    FillMode = Solid;
	CullMode = None;
};

//-----------------------------------------------------------------------------------------
// Vertex shader input structure
//-----------------------------------------------------------------------------------------
struct VSIn
{
	float3 PosLocal	: POSITION;
};

//-----------------------------------------------------------------------------------------
// Pixel shader input
//-----------------------------------------------------------------------------------------
struct PSIn
{
	float4 PosH 	: SV_POSITION;
};

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
PSIn VS( VSIn input)
{
	PSIn output = (PSIn)0;
	output.PosH = mul (float4(input.PosLocal, 1.0f), gWVP);
	return output;
}

float4 PS(PSIn input) : SV_TARGET
{
	return color;
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
		SetRasterizerState( rasterState );
	}
}