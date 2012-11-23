/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/8/2012 												*/
/************************************************************************/

#ifndef AABB_H
#define AABB_H

#include "stdafx.h"
#include "Object.h"
#include "Buffer.h"
#include "Shader.h"
#include "MinMax.h"
#include <vector>

using namespace std;

class AABB : public Object
{
private:
	Buffer					*m_vertexBuffer,
							*m_indexBuffer;
	Shader*					m_shader;
	MinMax					m_minMax;
	D3DXCOLOR				m_color;
	ID3D11DeviceContext*	m_deviceContext;
	bool					m_ownMatrix;
public:
	AABB(ID3D11DeviceContext*, D3DXCOLOR, MinMax, Buffer*,Buffer*, Shader*, bool p_useOwnMatrix=false);
	~AABB();
	HRESULT Update(float p_deltaTime);
	HRESULT Render(D3DXMATRIX p_matrix);
	D3DXVECTOR3 GetMinPos();
	D3DXVECTOR3 GetMaxPos();
	void SetMinPos(const D3DXVECTOR3 &newMin);
	void SetMaxPos(const D3DXVECTOR3 &newMax);
	void	ApplyMatrixToMinMax(float p_scale, D3DXVECTOR3 p_transform);
	Buffer*	GetVertexBuffer();
	Buffer* GetIndexBuffer();
	void	UseOwnMatrix(bool);
};
#endif // AABB_H__
