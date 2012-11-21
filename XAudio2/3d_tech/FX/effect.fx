float4x4 transform;    // a matrix to store the transform

SamplerState texSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

// a struct for the vertex shader return value
struct VSOut
{
	float4 Col : COLOR;    // vertex color
	float4 Pos : SV_POSITION;    // vertex screen coordinates
};

struct PSOut
{
	float4 albedo: SV_TARGET0;	//Different Buffers
	float4 normal: SV_TARGET1;	//
};
// the vertex shader
VSOut VS(float4 Col : COLOR, float4 Pos : POSITION)
{
	VSOut Output;
	Output.Pos = mul(Pos, transform);    // transform the vertex from 3D to 2D
	Output.Col = Col;    // set the vertex color to the input's color

	return Output;    // send the modified vertex data to the Rasterizer Stage
}


PSOut PS(VSOut input)
{
	PSOut output;
	
	output.albedo = input.Col;
	output.normal = input.Col;

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