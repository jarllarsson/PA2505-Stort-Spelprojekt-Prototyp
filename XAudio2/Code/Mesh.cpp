#include "Mesh.h"


Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, Shader* pShader, 
	ID3D11ShaderResourceView* pAlbedoTexture, ObjData* p_data, 
	D3DXVECTOR3 pos, AABB* p_boundingBox) 
	: Object(pos)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;
	mShader			= pShader;
	mAlbedoTexture	= pAlbedoTexture;
	m_boundingBox   = p_boundingBox;

	setScale(0.05f);

	D3DXVECTOR3 newMin = m_boundingBox->GetMinPos()*scaleLevel;
	D3DXVECTOR3 newMax = m_boundingBox->GetMaxPos()*scaleLevel;

	m_boundingBox->SetMinPos(D3DXVECTOR3(newMin));
	m_boundingBox->SetMaxPos(D3DXVECTOR3(newMax));

	m_boundingBox->ApplyMatrixToMinMax(scaleLevel,pos);

	m_vertexBuffer = p_data->data->vertexBuffer;
	m_indiciesBuffer = p_data->data->indexBuffer;

	int dir = -1;
	if(rand() % 2)
		dir = 1;

	float zOffset = (rand() % 100+1)/100.f;
	float yOffset = (rand() % 100+1)/100.f;

	rotY = yOffset;
	rotZ = zOffset;

	m_isVisible = false;
	m_textureOffset = 0;

	m_dir = dir;
}

Mesh::~Mesh()
{
	SAFE_DELETE(m_boundingBox);
}

HRESULT Mesh::Update(float deltaTime)
{
	HRESULT hr = S_OK;

	rotY += m_dir * deltaTime/2.0f;
	rotZ += m_dir * deltaTime/2.0f;
	m_textureOffset += 0.1f*deltaTime;
	return hr;
}

HRESULT Mesh::Render(D3DXMATRIX viewProjection)
{
	HRESULT hr = S_OK;

	D3DXMATRIX T, worldViewProjection, world, lol;
	D3DXMatrixScaling(&T,1.0f,1.0f,1.0f);
	D3DXMatrixTranslation(&lol, m_textureOffset, m_textureOffset, 0);
	T = lol * T;

	world = getFinalMatrix();

	worldViewProjection = world * viewProjection;

	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gWVP",worldViewProjection);
	mShader->SetMatrix("texMat", T);
	mShader->SetResource("gAlbedoMap",mAlbedoTexture);
	mShader->Apply(0);

	m_vertexBuffer->Apply(0);
	m_indiciesBuffer->Apply(0);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(m_indiciesBuffer->GetElementCount(),0,0);

	m_isVisible = false;

	return hr;
}

HRESULT Mesh::RenderShadowMap( Shader* p_shader, D3DXMATRIX p_viewProjection )
{
	D3DXMATRIX wvp;
	wvp = getFinalMatrix()* p_viewProjection;

	p_shader->SetMatrix("gLightWVP", wvp);
	p_shader->Apply(0);

	m_vertexBuffer->Apply(0);
	m_indiciesBuffer->Apply(0);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(m_indiciesBuffer->GetElementCount(),0,0);
	return S_OK;
}

HRESULT Mesh::RenderDebug(D3DXMATRIX p_viewProjection)
{
	HRESULT hr = S_OK;

	D3DXMATRIX worldViewProjection,world;

	world = getFinalMatrix();

	worldViewProjection = world*p_viewProjection;

	m_boundingBox->Render(worldViewProjection);

	return hr;
}

AABB* Mesh::GetAABB()
{
	return m_boundingBox;
}

void Mesh::SetVisible()
{
	m_isVisible = true;
}

bool Mesh::GetIsVisible()
{
	return m_isVisible;
}
