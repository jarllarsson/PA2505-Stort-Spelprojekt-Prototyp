/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/25/2012 												*/
/************************************************************************/

#ifndef POINTLIGHT_H__
#define POINTLIGHT_H__

#include "stdafx.h"
#include "AABB.h"
#include "Shader.h"
#include "LightInfo.h"
#include "MovementComp.h"
#include "Object.h"
#include "MeshBuffers.h"

class PointLight : public Object
{
private:
	
	LightInfo				mInformation[1];
	MeshBuffers*			mLightGeometry;
	Shader*					mShader;
	D3DXVECTOR3				mLightAtt;
	D3DXCOLOR				m_color;
	float					dir;
	ID3D11DeviceContext*	mDeviceContext;
	MovementComp*			mMovement;
	D3DXMATRIX				m_scale;
	ID3D11RasterizerState*	m_rasterState;
	AABB*					m_debugData;
	ID3D11BlendState*		m_blendState;
public:
	PointLight();
	~PointLight();
	HRESULT Init(ID3D11BlendState* p_blendState, ID3D11RasterizerState* p_rasterState, ID3D11DeviceContext* deviceContext,MeshBuffers* lightGeometry, AABB* p_debugData,Shader* shader,D3DXVECTOR3 pos, D3DXCOLOR diffuse, float range);
	HRESULT Update(float deltaTime);
	HRESULT Draw(D3DXMATRIX p_viewProjection, D3DXMATRIX viewProjectionInverse);
	HRESULT Render(D3DXMATRIX viewProjection);
	HRESULT	RenderDebug(D3DXMATRIX p_viewProject);
	D3DXCOLOR* GetColor();
};
#endif // POINTLIGHT_H__
