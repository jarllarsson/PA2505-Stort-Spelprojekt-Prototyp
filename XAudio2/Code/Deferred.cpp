#include "Deferred.h"

Deferred::Deferred()
{
	for (int i = 0; i < NUMBUFFERS; i++)
	{
		gBuffer[i]						= NULL;
		gBufferShaderResource[i]		= NULL;
	}
	finalImage					= NULL;
	lightPass					= NULL;
	SSAOPass					= NULL;
	depthStencilView			= NULL;
	device						= NULL;
	deviceContext				= NULL;
	swapChain					= NULL;
}

Deferred::~Deferred()
{
	SAFE_RELEASE(finalImage);
	SAFE_RELEASE(lightPass);
	SAFE_RELEASE(depthStencilView);

	for (unsigned int i = 0; i < NUMBUFFERS; i++)
	{
		SAFE_RELEASE(gBuffer[i]);
		SAFE_RELEASE(gBufferShaderResource[i]);
	}
	SAFE_DELETE(mVertexBuffer);
}

HRESULT Deferred::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain,ShaderFactory* shaderFactory)
{
	HRESULT hr = S_OK;

	this->device = device;
	this->deviceContext = deviceContext;
	this->swapChain = swapChain;

	if(FAILED(InitDepthBuffer()))
		return E_FAIL;

	if (FAILED(InitBackBuffer()))
		return E_FAIL;
	
	if(FAILED(InitGBuffers()))
		return E_FAIL;

	if(FAILED(InitSSAOPass()))
		return E_FAIL;

	if(FAILED(InitLightPass()))
		return E_FAIL;

	if(FAILED(InitQuad()))
		return E_FAIL;

	mShader = shaderFactory->CreateDeferredShader();

	return hr;
}
/************************************************************************/
/* Creates the depth buffer so that it also can be utilized from the	*/
/* shader.																*/
/************************************************************************/
HRESULT Deferred::InitDepthBuffer()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = SCREEN_WIDTH;
	depthStencilDesc.Height = SCREEN_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&depthStencilDesc,NULL,&depthStencilTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthStencilTexture,&depthStencilViewDesc,&depthStencilView)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	ZeroMemory(&shaderResourceDesc,sizeof(shaderResourceDesc));
	shaderResourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(depthStencilTexture,&shaderResourceDesc,&gBufferShaderResource[NUMBUFFERS-1])))
		return E_FAIL;

	depthStencilTexture->Release();

	return hr;
}
HRESULT Deferred::InitBackBuffer()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&backBuffer);
	hr = device->CreateRenderTargetView(backBuffer,NULL,&finalImage);
	backBuffer->Release();

	deviceContext->OMSetRenderTargets(1,&finalImage,NULL);

	return hr;
}
/************************************************************************/
/*	Creates the other buffers so they can be utilized by the shader	and	*/
/*	other parts of the code.											*/
/************************************************************************/
HRESULT Deferred::InitGBuffers()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* gBufferTextures[NUMBUFFERS];
	D3D11_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc) );

	gBufferDesc.Width = SCREEN_WIDTH;
	gBufferDesc.Height = SCREEN_HEIGHT;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	for (unsigned int i = 0; i < NUMBUFFERS; i++){
		hr = device->CreateTexture2D(&gBufferDesc,NULL,&gBufferTextures[i]);		
		if (hr != S_OK)
			return hr;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (unsigned int i = 0; i < NUMBUFFERS; i++){
		hr = device->CreateRenderTargetView(gBufferTextures[i], &renderTargetViewDesc, &gBuffer[i]);
		if (hr != S_OK )
			return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = gBufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	/* !!! Note that the for loop starts at 1 depthbuffer already in init !!! */
	for (unsigned int i = 0; i < NUMBUFFERS-1; i++){
		hr = device->CreateShaderResourceView(gBufferTextures[i],&shaderResourceDesc,&gBufferShaderResource[i]);
		gBufferTextures[i]->Release();
		if (hr != S_OK )
			return hr;
	}
	return hr;
}

HRESULT Deferred::InitSSAOPass()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* ssaoPassTexture;
	D3D11_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc));
	gBufferDesc.Width = SCREEN_WIDTH;
	gBufferDesc.Height = SCREEN_HEIGHT;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R16_FLOAT;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&gBufferDesc,NULL,&ssaoPassTexture);		
	if (hr != S_OK)
		return hr;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateRenderTargetView(ssaoPassTexture, &renderTargetViewDesc, &SSAOPass)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = gBufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(device->CreateShaderResourceView(ssaoPassTexture,&shaderResourceDesc,&SSAOPassResource)))
		return E_FAIL;
	
	ssaoPassTexture->Release();
	
	return hr;
}

HRESULT Deferred::InitLightPass()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* lightPassTexture;
	D3D11_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc) );

	gBufferDesc.Width = SCREEN_WIDTH;
	gBufferDesc.Height = SCREEN_HEIGHT;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&gBufferDesc,NULL,&lightPassTexture);		
	if (hr != S_OK)
		return hr;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(lightPassTexture, &renderTargetViewDesc, &lightPass);
	if (hr != S_OK )
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = gBufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(lightPassTexture,&shaderResourceDesc,&lightPassResource);
	lightPassTexture->Release();

	if (hr != S_OK )
		return hr;

	return hr;
}
HRESULT Deferred::InitQuad()
{
	HRESULT hr = S_OK;

	//  [6/13/2012 Hoodad]
	VERTEX mesh[] = {
		{D3DXVECTOR3( 1, -1, 0),	D3DXVECTOR2(1,1)},
		{D3DXVECTOR3( -1, -1, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( 1, 1,	0),		D3DXVECTOR2(1,0)},

		{D3DXVECTOR3( -1, -1, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( 1, 1, 0),		D3DXVECTOR2(1,0)},
		{D3DXVECTOR3( -1, 1, 0),	D3DXVECTOR2(0,0)},
	};

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(VERTEX);
	bufferDesc.InitData = mesh;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	mVertexBuffer = new Buffer();
	hr = mVertexBuffer->Init(device,deviceContext,bufferDesc);

	return hr;
}
HRESULT Deferred::InitLights(ShaderFactory* shaderFactory)
{
	HRESULT hr = S_OK;

	//lightHandler = new LightHandler();
	//lightHandler->Init(device,deviceContext,shaderFactory->CreateLightShader());

	return hr;
}

void Deferred::ClearBuffers()
{	
	UnmapBuffers();

	deviceContext->ClearRenderTargetView(finalImage,BLACK);
	deviceContext->ClearRenderTargetView(lightPass,REALBLACK);
	deviceContext->ClearRenderTargetView(SSAOPass,WHITE);
	deviceContext->ClearDepthStencilView(depthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);

	for(unsigned int i = 0; i < NUMBUFFERS; i++)
	{
		deviceContext->ClearRenderTargetView(gBuffer[i],BLACK);
	}

	// !!! SPECIAL FOR ALPHA LAYER IN ALBEDO BUFFER !!!
	deviceContext->ClearRenderTargetView(gBuffer[1],REALBLACK);
}

void Deferred::RenderFrame(D3DXMATRIX p_viewProjInverse, D3DXMATRIX p_shadowViewProj, ID3D11ShaderResourceView* p_shadowMap, int p_shadowMapSize)
{
	deviceContext->OMSetRenderTargets(1,&finalImage,NULL);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	mVertexBuffer->Apply(0);

	mShader->SetResource("lightPass",lightPassResource);
	mShader->SetResource("SSAOPass", SSAOPassResource);
	mShader->SetResource("tex_gBuffer2", gBufferShaderResource[GDEPTH]);
	mShader->SetResource("shadowMap", p_shadowMap);
	mShader->SetMatrix("gViewProjectionInverse",p_viewProjInverse);
	mShader->SetMatrix("lightVP", p_shadowViewProj);
	mShader->SetFloat("SMAP_SIZE", p_shadowMapSize);
	mShader->SetFloat("SHADOW_EPSILON", 0.01f);

	mShader->Apply(0);
	deviceContext->Draw(6,0);

	mShader->SetResource("shadowMap",NULL);
	mShader->Apply(0);
}

void Deferred::RenderToGBuffers()
{
	deviceContext->OMSetRenderTargets(NUMBUFFERS,gBuffer,depthStencilView);
	
	char tmp[100];
	for(int i = 0; i < NUMBUFFERS;i++)
	{
		sprintf_s(tmp, sizeof(tmp), "tex_gBuffer%d",i);
		mShader->SetResource(tmp, gBufferShaderResource[i]);
	}
}

void Deferred::RenderLightPrePass(Shader* lightShader)
{
	lightShader->SetResource("tex_gBuffer0",	gBufferShaderResource[GNORMAL]);
	lightShader->SetResource("tex_gBuffer2",	gBufferShaderResource[GDEPTH]);
	deviceContext->OMSetRenderTargets(1,&lightPass,NULL);
}

void Deferred::RenderSSAOPass()
{
	deviceContext->OMSetRenderTargets(1,&SSAOPass,NULL);
}

void Deferred::RenderUnLitObjects()
{
	deviceContext->OMSetRenderTargets(1,&gBuffer[GALBEDO],NULL);
}

void Deferred::UnmapBuffers()
{
	char tmp[100];
	for(int i = 0; i < NUMBUFFERS;i++)
	{
		sprintf_s(tmp, sizeof(tmp), "tex_gBuffer%d",i);
		mShader->SetResource(tmp,NULL);
	}

	mShader->SetResource("lightPass",NULL);
	mShader->SetResource("SSAOPass",NULL);
	mShader->Apply(0);
}

ID3D11ShaderResourceView* Deferred::GetDepthBuffer()
{
	return gBufferShaderResource[GDEPTH];
}

ID3D11ShaderResourceView* Deferred::GetNormalBuffer()
{
	return gBufferShaderResource[GNORMAL];
}

ID3D11ShaderResourceView* Deferred::GetSSAOBuffer()
{
	return SSAOPassResource;
}

ID3D11ShaderResourceView* Deferred::GetLightBuffer()
{
	return lightPassResource;
}
