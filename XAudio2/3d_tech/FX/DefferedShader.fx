Texture2D tex_gBuffer0;	//DIFFUSE
Texture2D tex_gBuffer1;	//ALBEDO
Texture2D tex_gBuffer2;	//DEPTH

struct VSOut
{
	float2 Tex	: TEXCOORD;
	float4 Pos	: SV_POSITION;
	float3 posW	: POSITION;
};

struct vertex
{
	uint id : SV_VertexID;
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

SamplerState sample
{
};

/*
*
*Create a quad over the whole screen and paint the results from the gbuffer onto this quad
*
*/
VSOut VS(vertex v)
{
	VSOut output;

	if(v.id == 0)
	{
		output.Pos = float4(-1.0f,-1.0f,0.0f,1.0f);
		output.Tex = float2(0.0f,1.0f);

		output.posW = float3(-1.0f,-1.0f, 0.0f);
	}
	else if(v.id == 1)
	{
		output.Pos = float4(-1.0f,1.0f,0.0f,1.0f);
		output.Tex = float2(0.0f,0.0f);

		output.posW = float3(-1.0f,1.0f, 0.0f);
	}
	else if(v.id == 2)
	{
		output.Pos = float4(1.0f,-1.0f,0.0f,1.0f);
		output.Tex = float2(1.0f,1.0f);

		output.posW = float3(-1.0f,1.0f, 0.0f);
	}
	else
	{
		output.Pos = float4(1.0f,1.0f,0.0f,1.0f);
		output.Tex = float2(1.0f,0.0f);

		output.posW = float3(1.0f,1.0f, 0.0f);
	}

	return output;
}

float4 PS(VSOut input) : SV_TARGET
{
	

	/*albedo*/
	float4 albedo = tex_gBuffer1.Sample(sample, input.Tex);
	float4 diffuse = tex_gBuffer0.Sample(sample, input.Tex);

	uint3 index = uint3(input.Pos.x,input.Pos.y,0);
	float val = tex_gBuffer2.Load(index).r; //Sample from depthbuffer
	float depth = 1.0f-val*0.75f+0.2f;

	//float4 pixelColour = float4(val,val,val,1.0f);
	float4 pixelColour = float4(diffuse.x*depth,diffuse.y*depth,diffuse.z*depth,1.0f);

	return pixelColour;
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