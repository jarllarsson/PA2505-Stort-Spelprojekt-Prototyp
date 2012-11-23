/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/22/2012 												*/
/************************************************************************/

#ifndef SHADOWMAPS_H
#define SHADOWMAPS_H

#include "Camera.h"
#include "Shader.h"
#include "Frustum.h"

class ShadowMaps
{
private:
	ID3D11ShaderResourceView*	m_shadowResource;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext;
	ID3D11DepthStencilView*		m_depthStencilView;
	Shader*						m_shader;
	float						m_pitch;
	int							m_dir;
	Frustum*					m_frustum;
	int							m_mapSize;
	int							m_prevMapSize;
	bool						m_changedMap;
private:
	HRESULT InitDepthStencil();
public:
	ShadowMaps(	ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
				Shader* p_shader, Frustum* p_shadowFrustum, int p_mapSize);
	~ShadowMaps();
	D3DXMATRIX GetViewProjection();
	ID3D11ShaderResourceView* GetShadowResource();
	void ShadowBeginRender();
	void EndShadowRender();
	HRESULT Update(float p_deltaTime);
	HRESULT RenderDebug(D3DXMATRIX p_viewProjection);
	Shader* GetShadowShader();
	int* GetMapSizeRef();
	int  GetMapSize();
	bool CheckIfShadowMapHasChanged();
};
#endif // SHADOWMAPS_H