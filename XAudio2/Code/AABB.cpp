#include "AABB.h"

AABB::AABB(ID3D11DeviceContext* p_deviceContext, D3DXCOLOR p_color, MinMax p_minMax, Buffer* p_vertexBuffer, Buffer* p_indexBuffer, Shader* p_shader, bool p_useOwnMatrix)
	: Object(D3DXVECTOR3(0,0,0))
{
	m_color			= p_color;
	m_minMax		= p_minMax;
	m_vertexBuffer	= p_vertexBuffer;
	m_indexBuffer	= p_indexBuffer;
	m_shader		= p_shader;
	m_deviceContext = p_deviceContext;
	m_ownMatrix		= p_useOwnMatrix;
}
AABB::~AABB()
{
	SAFE_DELETE(m_vertexBuffer);
	SAFE_DELETE(m_indexBuffer);
}

HRESULT AABB::Update(float p_deltaTime)
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT AABB::Render(D3DXMATRIX p_matrix)
{
	HRESULT hr = S_OK;

	D3DXMATRIX wvp;

	if(m_ownMatrix)
		wvp = getFinalMatrix()*p_matrix;
	else
		wvp = p_matrix;

	m_shader->SetMatrix("gWVP", wvp);
	m_shader->SetFloat4("color", D3DXVECTOR4(m_color.r,m_color.g,m_color.b,m_color.a));
	m_shader->Apply(0);

	m_vertexBuffer->Apply(0);
	m_indexBuffer->Apply(0);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_deviceContext->DrawIndexed(m_indexBuffer->GetElementCount(),0,0);

	return hr;
}

D3DXVECTOR3 AABB::GetMinPos()
{
	return  m_minMax.min;
}

D3DXVECTOR3 AABB::GetMaxPos()
{
	return m_minMax.max;
}

void AABB::ApplyMatrixToMinMax(float p_scale, D3DXVECTOR3 pos)
{
	//TODO: Use code listed in Frank D. Luna on page 486
	D3DXVECTOR4 newMin = D3DXVECTOR4( m_minMax.min,1.0f);
	D3DXVECTOR4 newMax = D3DXVECTOR4( m_minMax.max,1.0f);

	D3DXVec4Scale(&newMin,&newMin, p_scale);
	D3DXVec4Scale(&newMax,&newMax, p_scale);

	m_minMax.min = D3DXVECTOR3(newMin.x+pos.x,newMin.y+pos.y,newMin.z+pos.z);
	m_minMax.max = D3DXVECTOR3(newMax.x+pos.x,newMax.y+pos.y,newMax.z+pos.z);
}

Buffer* AABB::GetVertexBuffer()
{
	return m_vertexBuffer;
}

Buffer* AABB::GetIndexBuffer()
{
	return m_indexBuffer;
}

void AABB::UseOwnMatrix( bool p_useOwnMatrix )
{
	m_ownMatrix = p_useOwnMatrix;
}

void AABB::SetMinPos(const D3DXVECTOR3 &newMin)
{
	m_minMax.min = newMin;
}

void AABB::SetMaxPos( const D3DXVECTOR3 &newMax )
{
	m_minMax.max = newMax;
}
