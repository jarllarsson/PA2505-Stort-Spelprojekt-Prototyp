#include "ShadowMaps.h"

ShadowMaps::ShadowMaps(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
						Shader* p_shader, Frustum* p_shadowFrustum,	int p_mapSize)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_pitch = 0;
	m_dir = 1;
	m_shader = p_shader;
	m_frustum = p_shadowFrustum;
	m_mapSize = p_mapSize;
	HRESULT hr = InitDepthStencil();
	m_prevMapSize = m_mapSize;
	m_changedMap	= false;
}

ShadowMaps::~ShadowMaps()
{
	SAFE_RELEASE(m_shadowResource);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_DELETE(m_frustum);
}

HRESULT ShadowMaps::InitDepthStencil()
{
	ID3D11Texture2D* shadowMapTexture;
	D3D11_TEXTURE2D_DESC descDepthTexture;
	descDepthTexture.Width = m_mapSize;
	descDepthTexture.Height = m_mapSize;
	descDepthTexture.MipLevels = 1;
	descDepthTexture.ArraySize = 1;
	descDepthTexture.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepthTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepthTexture.SampleDesc.Count = 1;
	descDepthTexture.SampleDesc.Quality = 0;
	descDepthTexture.CPUAccessFlags = 0;
	descDepthTexture.MiscFlags = 0;

	if (FAILED(m_device->CreateTexture2D(&descDepthTexture,NULL,&shadowMapTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;


	if (FAILED(m_device->CreateDepthStencilView(shadowMapTexture,&depthStencilViewDesc,&m_depthStencilView)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	ZeroMemory(&shaderResourceDesc,sizeof(shaderResourceDesc));
	shaderResourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_device->CreateShaderResourceView(shadowMapTexture,&shaderResourceDesc,&m_shadowResource)))
		return E_FAIL;

	shadowMapTexture->Release();

	return S_OK;
}

D3DXMATRIX ShadowMaps::GetViewProjection()
{
	return m_frustum->GetFrustumCamera()->getViewProjMatrix();
}

ID3D11ShaderResourceView* ShadowMaps::GetShadowResource()
{
	return m_shadowResource;
}

void ShadowMaps::ShadowBeginRender()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Height = (float)m_mapSize;
	vp.Width = (float)m_mapSize;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	m_deviceContext->RSSetViewports(1,&vp);

	if(m_mapSize != m_prevMapSize)
	{
		m_prevMapSize = m_mapSize;
		InitDepthStencil();
		m_changedMap = true;
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
	ID3D11RenderTargetView* temp = NULL;
	m_deviceContext->OMSetRenderTargets(1,&temp,m_depthStencilView);
}

void ShadowMaps::EndShadowRender()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Height = SCREEN_HEIGHT;
	vp.Width = SCREEN_WIDTH;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	m_deviceContext->RSSetViewports(1,&vp);

	ID3D11RenderTargetView* temp = NULL;
	m_deviceContext->OMSetRenderTargets(1,&temp, NULL);
}

HRESULT ShadowMaps::Update( float p_deltaTime )
{
	m_frustum->Update(p_deltaTime);
	m_changedMap = false;
	return S_OK;
}

Shader* ShadowMaps::GetShadowShader()
{
	return m_shader;
}

HRESULT ShadowMaps::RenderDebug( D3DXMATRIX p_viewProjection )
{
	m_frustum->Render(p_viewProjection);

	return S_OK;
}

int* ShadowMaps::GetMapSizeRef()
{
	return &m_mapSize;
}

int ShadowMaps::GetMapSize()
{
	return m_mapSize;
}

bool ShadowMaps::CheckIfShadowMapHasChanged()
{
	return m_changedMap;
}