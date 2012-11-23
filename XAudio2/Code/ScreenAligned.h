/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/21/2012 												*/
/************************************************************************/

#ifndef SCREENALIGNED_H
#define SCREENALIGNED_H

#include "Buffer.h"
#include "Shader.h"
#include "EventInterface.h"

class SA_event;

class ScreenAligned
{
private:
	ID3D11ShaderResourceView	*m_sampleTarget;
	ID3D11DeviceContext			*m_deviceContext;
	Buffer						*m_vertexBuffer;
	Shader						*m_shader;
	bool						m_draw;
	SA_event					*m_eventListener;
	D3DXVECTOR4					m_activeSampleChannels;
	int							m_inverted;
public:
	ScreenAligned(ID3D11DeviceContext* p_deviceContext, ID3D11ShaderResourceView* p_sampleTarget, 
					Shader* p_shader, Buffer* p_vertexBuffer, D3DXVECTOR4 p_activeSampleChannels, 
					bool p_draw=false, int p_invertedSampling=0);
	~ScreenAligned();
	void SwitchRenderState();
	HRESULT Update(float p_deltaTime);
	HRESULT Render();
	void UpdateShaderResource(ID3D11ShaderResourceView* p_sampleTarget);
	EventInterface* getSwitchRenderSE();
};
#endif // SCREENALIGNED_H