cbuffer perFrame
{
	matrix mat_world;
};

struct struct_vsRocket
{
	float4 pos			: SV_POSITION;
	float4 color		: COLOR;
	float2 texCoord		: TEXCOORD;
};

Texture2D tex_default;

SamplerState sample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
struct_vsRocket vsRocket(float3 pos : POSITION, float4 color : COLOR, float2 texCoord : TEXCOORD)
{
	struct_vsRocket output;
	pos.z = 0.99f - pos.z;
	output.pos = mul(float4(pos, 1.0f), mat_world);
	output.color = color;
	output.texCoord = texCoord;

	return output;
}


float4 psRocket(struct_vsRocket input) : SV_TARGET
{
	float2 uv = input.texCoord;
	//uv.y *= -1.0f;
	float4 Col = tex_default.Sample(sample, uv);
	return Col;
}

technique10 Technique_0 //For the librocket menu
{
	pass Pass_0
	{
		SetVertexShader(CompileShader(vs_4_0, vsRocket()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, psRocket()));
	}
}