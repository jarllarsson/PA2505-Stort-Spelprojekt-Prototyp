#ifndef DeferredRender_h__
#define DeferredRender_h__

#include <d3d10.h>
#include <d3dx10.h>

struct VERTEX 
{
	D3DXVECTOR3 pos;
};

class DeferredRender
{
private:
	static const UINT countBuff = 3;

	ID3D10RenderTargetView* rtv;

	ID3D10RenderTargetView* gBuffer[countBuff];
	ID3D10ShaderResourceView* gBufferShaderResource[countBuff];
	ID3D10EffectShaderResourceVariable* gBufferShaderEffect[countBuff];

	ID3D10DepthStencilView* dsv;			// the pointer to the depth stencil view
	ID3D10Buffer* pBuffer;
	ID3D10Effect* pEffect;
	ID3D10EffectTechnique* pTechnique;
	ID3D10EffectPass* pPass;
	ID3D10InputLayout* pVertexLayout;

	/*Extern pointers should not be cleaned from this class*/
	ID3D10Device* device;
	IDXGISwapChain* swapchain;

private:
	void init_depthbuffer();
	void init_gBuffers();
	void init_defferedShader();
	void clear_gBuffers();
	void render_frame();
protected:
public:
	DeferredRender(ID3D10Device* pDevice, IDXGISwapChain* pSwapchain);
	~DeferredRender();
	void init();
	void draw();

};
#endif // DeferredRender_h__
