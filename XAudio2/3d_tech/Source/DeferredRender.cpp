
#include "DeferredRender.h"
#include "Globals.h"
#include <iostream>

DeferredRender::DeferredRender(ID3D10Device* pDevice, IDXGISwapChain* pSwapchain)
{
	device = pDevice;
	swapchain = pSwapchain;
}
DeferredRender::~DeferredRender()
{
	if(rtv)
		rtv->Release();
	
	for(int i=0; i < countBuff; i++)
	{
		if(gBuffer[i])
		{
			gBuffer[i]->Release();
			gBufferShaderResource[i]->Release();
		}
	}
	
	if(dsv)
		dsv->Release();
	if(pBuffer)
		pBuffer->Release();
	if(pEffect)
		pEffect->Release();
}
void DeferredRender::init()
{
	init_depthbuffer();
	init_gBuffers();
	init_defferedShader();
}

void DeferredRender::init_depthbuffer()
{
	ID3D10Texture2D* depthStencilTexture;
	D3D10_TEXTURE2D_DESC depthStencilTexDesc;
	depthStencilTexDesc.Width = SCREEN_WIDTH;
	depthStencilTexDesc.Height = SCREEN_HEIGHT;
	depthStencilTexDesc.MipLevels = 1;
	depthStencilTexDesc.ArraySize = 1;
	depthStencilTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilTexDesc.Usage = D3D10_USAGE_DEFAULT;
	depthStencilTexDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	depthStencilTexDesc.SampleDesc.Count = 1;
	depthStencilTexDesc.CPUAccessFlags = false;
	depthStencilTexDesc.MiscFlags = 0;

	HRESULT textResult = device->CreateTexture2D(&depthStencilTexDesc,NULL,&depthStencilTexture);

	if(textResult != S_OK){
		MessageBox(0,L"Creating the depthstenciltexture faild!",0,0);
		return;
	}

	D3D10_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Texture2D.MipSlice = 0;

	HRESULT viewResult = device->CreateDepthStencilView(depthStencilTexture,&depthDesc,&dsv);

	if(viewResult != S_OK){
		MessageBox(0, L"Creating the depthstencilview failed!",0,0);
		return;
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC resDesc;
	ZeroMemory(&resDesc,sizeof(resDesc));
	resDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	resDesc.Texture2D.MostDetailedMip = 0;
	resDesc.Texture2D.MipLevels = 1;

	HRESULT depthResourceViewResult = device->CreateShaderResourceView(depthStencilTexture,&resDesc,&gBufferShaderResource[2]);

	if (depthResourceViewResult != S_OK){
		MessageBox(0, L"Creating the DepthStencilShaderResourceView failed!",0,0);
		return;
	}

	depthStencilTexture->Release();
}
void DeferredRender::init_gBuffers()
{
	// get the address of the back buffer and use it to create the render target
	ID3D10Texture2D* backBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);
	//rtv is the backbuffer
	device->CreateRenderTargetView(backBuffer, NULL, &rtv);
	backBuffer->Release();

	// set the back buffer as the render target
	device->OMSetRenderTargets(1, &rtv, NULL);

	HRESULT textureResult;

	D3D10_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc) );

	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gBufferDesc.Width =  SCREEN_WIDTH;
	gBufferDesc.Height = SCREEN_HEIGHT;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	ID3D10Texture2D* tex_gBuffer[countBuff];

	for(int i = 0; i < countBuff; i++)
		textureResult = device->CreateTexture2D(&gBufferDesc, NULL, &tex_gBuffer[i]);

	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for(int i = 0; i < countBuff; i++)
		textureResult = device->CreateRenderTargetView(tex_gBuffer[i], &renderTargetViewDesc, &gBuffer[i]);

	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = gBufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	//WARNING countBuff-1
	//last buffer is depth buffer!
	for(int i = 0; i < countBuff-1; i++)
	{
		textureResult = device->CreateShaderResourceView(tex_gBuffer[i], &shaderResourceDesc, &gBufferShaderResource[i]);
		tex_gBuffer[i]->Release();
	}

	D3D10_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D10_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(VERTEX)*4;
	bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	bufferDesc.MiscFlags = 0;
	device->CreateBuffer(&bufferDesc, NULL, &pBuffer);
}

void DeferredRender::init_defferedShader()
{
	ID3D10Blob *effectBlob = 0, *errorsBlob = 0;

	HRESULT hr;
	hr = D3DX10CompileFromFile(
		L"FX//DefferedShader.fx", 0, 0, 0, "fx_4_0",0,0,0, &effectBlob, &errorsBlob, &hr);

	if(errorsBlob)
	{
		MessageBoxA(0,(char*)errorsBlob->GetBufferPointer(),0,0);
		errorsBlob->Release();
	}

	D3DX10CreateEffectFromMemory( 
		effectBlob->GetBufferPointer(),
		effectBlob->GetBufferSize(),
		0,
		0,
		0,
		0,
		0,
		0,
		device,
		0,
		0,
		&pEffect,
		&errorsBlob,
		&hr);

	effectBlob->Release();

	pTechnique = pEffect->GetTechniqueByIndex(0);
	pPass = pTechnique->GetPassByIndex(0);

	char tmp[100];
	for(int i = 0; i < countBuff;i++)
	{
		sprintf_s(tmp, sizeof(tmp), "tex_gBuffer%d",i);
		gBufferShaderEffect[i] = pEffect->GetVariableByName(tmp)->AsShaderResource();
		gBufferShaderEffect[i]->SetResource(gBufferShaderResource[i]);
	}
}
void DeferredRender::draw()
{
	render_frame();
	swapchain->Present(0,0);

	clear_gBuffers();
}
void DeferredRender::clear_gBuffers()
{
	device->ClearRenderTargetView(rtv,BLACK);
	device->ClearDepthStencilView(dsv,D3D10_CLEAR_DEPTH,1.0f,0);

	for (int i=0;i<countBuff-1;i++)
	{
		device->ClearRenderTargetView(gBuffer[i],BLACK);
	}
	device->OMSetRenderTargets(countBuff,gBuffer,dsv);
}

void DeferredRender::render_frame()
{
	device->OMSetRenderTargets(1,&rtv,NULL);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	device->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);

	device->IASetInputLayout(NULL);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pPass->Apply(0);

	device->Draw(4,0);
}