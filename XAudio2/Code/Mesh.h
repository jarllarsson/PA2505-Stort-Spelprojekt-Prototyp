#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include "Shader.h"
#include "Vertex.h"
#include "Object.h"
#include "Buffer.h"
#include "AABB.h"
#include "ObjData.h"

class Mesh : public Object
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	Buffer					*m_vertexBuffer,
							*m_indiciesBuffer;
	ID3D11ShaderResourceView* mAlbedoTexture;

	Shader*					mShader;
	int						m_dir;
	AABB					*m_boundingBox;
	bool					m_isVisible;
	float					m_textureOffset;
public:
	Mesh(	ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, Shader* pShader, 
			ID3D11ShaderResourceView* pAlbedoTexture, ObjData* p_data, D3DXVECTOR3 pos, 
			AABB* p_boundingBox);
	~Mesh();
	HRESULT	Render(D3DXMATRIX viewProjection);
	HRESULT Update(float deltaTime);
	HRESULT RenderDebug(D3DXMATRIX p_viewProjection);
	HRESULT RenderShadowMap(Shader* p_shader, D3DXMATRIX p_viewProjection);
	AABB*	GetAABB();
	bool	GetIsVisible();
	void	SetVisible();
};

#endif