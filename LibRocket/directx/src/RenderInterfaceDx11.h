#ifndef RENDERINTERFACEDX10_H
#define RENDERINTERFACEDX10_H

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>
#include <vector>
#include <Windows.h>
#include <D3DCommon.h>
#include <d3dCompiler.h>

#include <Rocket/Core.h>

#include "Utils.h"
#include "VertexTexture.h"
#include "Texture.h"

using namespace std;

// The internal format of the vertex we use for rendering Rocket geometry. 
// We could optimize space by having a second
// untextured vertex for use when rendering colored borders and backgrounds.
struct VertexRocket
{
	D3DXVECTOR3 pos;
	D3DXCOLOR	color;
	D3DXVECTOR2 texCoord;
};

// This structure is created for each set of geometry that Rocket compiles.
// It stores the vertex and index buffers and
// the texture associated with the geometry, if one was specified.
struct RocketDx11CompiledGeometry
{
	vector<VertexRocket> vertices;
	DWORD num_vertices;

	vector<unsigned int> indices;
	DWORD num_primitives;

	Texture* texture;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

class RenderInterfaceDx11 : public Rocket::Core::RenderInterface
{
private:
	typedef VertexRocket VertexType;	//change this per implementation

	//==============================================
	//Added for dx10
	//==============================================
	//States
	ID3D11RasterizerState* rs_scissorsOn;
	ID3D11RasterizerState* rs_scissorsOff;
	ID3D11BlendState* bs_normal;

	//FX
	ID3DX11Effect* effect;			
	ID3DX11EffectTechnique* technique;  
	ID3DX11EffectPass* pass;
	D3DX11_PASS_DESC PassDesc;	
	int techNr;
	int passNr;

	//input layout
	D3D11_INPUT_ELEMENT_DESC* layoutDesc;
	UINT numElements;
	ID3D11InputLayout* inputLayout;	

	//matrices for transformations
	ID3DX11EffectMatrixVariable* fxVar_world;
	D3DXMATRIX mat_world;

	/// Default textures
	Texture* defaultTex;

	//////////////////////////////////////////////////////////////////////////
	HWND		m_hWnd;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	ID3DX11Effect* m_effect;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;    //depth stencil view - z/depth buffer
	IDXGISwapChain* m_swapchain;
	D3D11_VIEWPORT m_viewport;
	int m_height;
	int m_width;
private:

	virtual void initPipeline();
	virtual void initFxVars();
	virtual void initDefaultTex();
	virtual void initDevice();
	virtual void initEffect();

public:
	RenderInterfaceDx11(HWND p_hWnd, int _techNr, int _passNr,
						int p_width, int p_height);
	virtual ~RenderInterfaceDx11();

	/// Called by Rocket when it wants to render geometry that it does not wish to optimize.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for the foreseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

	/// Returns the native horizontal texel offset for the renderer.
	float GetHorizontalTexelOffset();
	/// Returns the native vertical texel offset for the renderer.
	float GetVerticalTexelOffset();

	//==============================================
	//Added for dx10
	//==============================================
	void defineInputElementDesc();
	void defineBlendState();

	//////////////////////////////////////////////////////////////////////////
	void beginFrame();
	void presentFrame();
};

#endif //RENDERINTERFACEDX10_H