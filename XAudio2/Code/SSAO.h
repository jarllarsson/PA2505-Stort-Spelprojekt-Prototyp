/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/19/2012 												*/
/************************************************************************/

#ifndef SSAO_H
#define SSAO_H

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "SSAO_parms.h"

class SSAO
{
private:
	ID3D11Device				*m_device;
	ID3D11DeviceContext			*m_deviceContext;

	ID3D11ShaderResourceView	*m_randomNormals;
	
	Shader						*m_shader;
	Buffer						*m_buffer;

	SSAO_parms					m_SSAOInfo;
public:
	SSAO(ID3D11Device* p_deivce, ID3D11DeviceContext* p_deviceContext, Shader* p_shader);
	~SSAO();
	void RenderPostProcessing(	D3DXMATRIX p_vp, D3DXMATRIX p_invP, 
								ID3D11ShaderResourceView* p_normalBuffer, ID3D11ShaderResourceView* p_depthBuffer);
	SSAO_parms*	GetSSAOParms();
};
#endif // SSAO_H