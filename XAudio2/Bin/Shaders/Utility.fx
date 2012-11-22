/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/27/2012 												*/
/************************************************************************/


float3 ConvertSampledNormal(float4 sampledNormal)
{
	return float3(2.0f * sampledNormal.xyz - 1.0f);
}

float3 GetWorldPosition( float2 texCoord, float depthValue, matrix invVP)
{
	float4 newTexCoord = float4 (  	texCoord.x*2.0f-1.0f, 
									(1.0f-texCoord.y)*2.0f-1.0f,
									depthValue, 
									1.0f);
	float4 worldPos;	
	worldPos	= mul(newTexCoord, invVP);
	
	return worldPos.xyz/worldPos.w;
}