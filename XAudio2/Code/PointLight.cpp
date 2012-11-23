#include "PointLight.h"



PointLight::PointLight() : Object(D3DXVECTOR3(0,0,0))
{
	mLightGeometry = NULL;
}

PointLight::~PointLight()
{
	SAFE_DELETE(mLightGeometry->indexBuffer);
	SAFE_DELETE(mLightGeometry->vertexBuffer);
	SAFE_DELETE(mLightGeometry);
	SAFE_DELETE(mMovement);
	SAFE_DELETE(m_debugData);
}

HRESULT PointLight::Init(ID3D11BlendState* p_blendState, ID3D11RasterizerState* p_rasterState, 
						ID3D11DeviceContext* deviceContext,MeshBuffers* lightGeometry, 
						AABB* p_debugData,Shader* shader,D3DXVECTOR3 pos, D3DXCOLOR diffuse, float range)
{
	setPos(pos);
	m_blendState	= p_blendState;
	m_debugData		= p_debugData;
	m_rasterState	= p_rasterState;
	mLightGeometry	= lightGeometry;
	mShader			= shader;
	mDeviceContext	= deviceContext;
	mMovement		= new MovementComp(this,10,5);
	m_color			= diffuse;

	mInformation[0].pos		= getPos();
	mInformation[0].range	= range;
	mLightAtt				= D3DXVECTOR3(0.0f,0.9f,0.0f);
	dir						= 0.000001f;
	

	setScale(range);
	return S_OK;
}

HRESULT PointLight::Update(float deltaTime)
{
	mMovement->update(deltaTime);
	mInformation[0].pos = getPos();

	return S_OK;
}

HRESULT PointLight::Draw(D3DXMATRIX p_viewProjection, D3DXMATRIX viewProjectionInverse)
{
	float bfactor[] = {1.0f,1.0f,1.0f,1.0f};
	mDeviceContext->OMSetBlendState(m_blendState,bfactor,0xfff);
	mDeviceContext->RSSetState(m_rasterState);
	D3DXMATRIX wvp, world;
	world = getFinalMatrix();
	wvp = world*p_viewProjection;

	mShader->SetFloat4("lightColor",D3DXVECTOR4(m_color));
	mShader->SetFloat4("lightCenter",D3DXVECTOR4(mInformation[0].pos,1));
	mShader->SetFloat("range",mInformation[0].range);
	mShader->SetFloat4("lightAtt",D3DXVECTOR4(mLightAtt));
	mShader->SetMatrix("gWVP",wvp);
	mShader->SetMatrix("gInvVP",viewProjectionInverse);
	mShader->SetFloat4("SCREENSIZE",D3DXVECTOR4((float)SCREEN_WIDTH,(float)SCREEN_HEIGHT,0,0));
	
	mShader->Apply(0);
	mLightGeometry->vertexBuffer->Apply(0);
	mLightGeometry->indexBuffer->Apply(0);
	
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(mLightGeometry->indexBuffer->GetElementCount(),0,0);
	
	return S_OK;
}

HRESULT PointLight::RenderDebug(D3DXMATRIX p_viewProject)
{
	D3DXMATRIX wvp,world;
	float temp = getScaleLevel();
	//setScale(1);
	world = getFinalMatrix();
	//setScale(temp);
	wvp = world*p_viewProject;

	m_debugData->Render(wvp);
	
	return S_OK;
}

HRESULT PointLight::Render(D3DXMATRIX viewProjection)
{
	return S_OK;
}

D3DXCOLOR* PointLight::GetColor()
{
	return &m_color;
}