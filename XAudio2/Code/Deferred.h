#ifndef Deferred_H_
#define Deferred_H

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "ShaderFactory.h"
#include "Camera.h"
#include "LightHandler.h"
#include "PointLight.h"

#define NUMBUFFERS 3
#define GNORMAL 0
#define GALBEDO 1
#define GDEPTH 2

struct VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex;
};


class Deferred
{
private:
	ID3D11RenderTargetView					*finalImage,
											*lightPass,
											*SSAOPass,
											*gBuffer[NUMBUFFERS];

	ID3D11ShaderResourceView				*lightPassResource,
											*SSAOPassResource,
											*gBufferShaderResource[NUMBUFFERS];

	ID3D11DepthStencilView*					depthStencilView;

	Shader*									mShader;
	Buffer*									mVertexBuffer;

	ID3D11Device*							device;
	ID3D11DeviceContext*					deviceContext;
	IDXGISwapChain*							swapChain;

	LightHandler*							lightHandler;
private:
	HRESULT InitDepthBuffer();
	HRESULT InitGBuffers();
	HRESULT InitQuad();
	HRESULT InitLights(ShaderFactory* shaderFactory);
	HRESULT	InitSSAOPass();
	HRESULT InitLightPass();
	HRESULT InitBackBuffer();
protected:
public:
	Deferred();
	~Deferred();
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain,ShaderFactory* shaderFactory);
	void	RenderFrame(D3DXMATRIX p_viewProjInverse, D3DXMATRIX p_shadowViewProj, ID3D11ShaderResourceView* p_shadowMap, int p_shadowMapSize);
	void	RenderToGBuffers();
	void	RenderLightPrePass(Shader* lightShader);
	void	RenderSSAOPass();
	void	RenderUnLitObjects();
	void	ClearBuffers();
	void	UnmapBuffers();
	ID3D11ShaderResourceView*	GetDepthBuffer();
	ID3D11ShaderResourceView*	GetNormalBuffer();
	ID3D11ShaderResourceView*	GetSSAOBuffer();
	ID3D11ShaderResourceView*	GetLightBuffer();
};
#endif // Deferred_h__
