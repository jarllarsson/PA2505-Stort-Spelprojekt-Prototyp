/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

//==================================================================
// DX10 conversion of DX9 sample code from libRocket. This has been 
// modified and is NOT the original. Use as you wish. Please Leave 
// this message and please don't say that it is your work 
//
// - BiceMaster
//==================================================================

#include "RenderInterfaceDx11.h"

//HACK: temp placement
char* FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}

RenderInterfaceDx11::RenderInterfaceDx11(HWND p_hWnd, int _techNr, int _passNr, 
										int p_width, int p_height)
{
	m_hWnd = p_hWnd;
	m_width = p_width;
	m_height = p_height;
	techNr = _techNr;
	passNr = _passNr;

	initDevice();
	initEffect();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = TRUE;	// Changed it from CounterClockwise false to true, since it otherwise will be culled
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	rasterizerDesc.ScissorEnable = false;
	HR(m_device->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOff));

	rasterizerDesc.ScissorEnable = true;
	HR(m_device->CreateRasterizerState(&rasterizerDesc, &rs_scissorsOn));

	m_context->RSSetState(rs_scissorsOff);

	defineInputElementDesc();
	defineBlendState();
	initPipeline();
	initFxVars();
	initDefaultTex();
}

RenderInterfaceDx11::~RenderInterfaceDx11()
{
	SAFE_RELEASE(rs_scissorsOn);
	SAFE_RELEASE(rs_scissorsOff);
	SAFE_RELEASE(inputLayout);
	delete layoutDesc;
	delete defaultTex;

	Rocket::Core::Shutdown();
}

// Called by Rocket when it wants to render geometry that it does not wish to optimize.
void RenderInterfaceDx11 :: RenderGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{

	// We've chosen to not support non-compiled geometry in the DirectX renderer. If you wanted to render non-compiled
	// geometry, for example for very small sections of geometry, you could use DrawIndexedPrimitiveUP or write to a
	// dynamic vertex buffer which is flushed when either the texture changes or compiled geometry is drawn.
}

// Called by Rocket when it wants to compile geometry it believes will be static for the foreseeable future.
Rocket::Core::CompiledGeometryHandle RenderInterfaceDx11 :: CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{

	// Construct a new RocketD3D9CompiledGeometry structure, which will be returned as the handle, and the buffers to store the geometry.
	RocketDx11CompiledGeometry* geometry = new RocketDx11CompiledGeometry;

	// Fill the vertex vector.
	VertexType _vertex;
	for (int i = 0; i < num_vertices; i++)
	{
		_vertex.pos		= D3DXVECTOR3(	vertices[i].position.x, 
										vertices[i].position.y,
										0						);

		_vertex.color	= D3DXCOLOR(	vertices[i].colour.red, 
										vertices[i].colour.green, 
										vertices[i].colour.blue, 
										vertices[i].colour.alpha);

		_vertex.texCoord = D3DXVECTOR2(	vertices[i].tex_coord[0], 
										vertices[i].tex_coord[1]);

		geometry->vertices.push_back(_vertex);
	}

	// Fill the index vector.
	geometry->indices.resize(num_indices);
	memcpy(&geometry->indices[0], indices, sizeof(unsigned int) * num_indices);

	//create and fill the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = num_vertices*sizeof(VertexType);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertBuffSubRes;
	vertBuffSubRes.pSysMem = &geometry->vertices[0];
	HR(m_device->CreateBuffer(&vbd, &vertBuffSubRes, &geometry->vertexBuffer));

	//create and fill the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = num_indices*sizeof(int);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indBuffSubRes;
	indBuffSubRes.pSysMem = &geometry->indices[0];
	HR(m_device->CreateBuffer(&ibd, &indBuffSubRes, &geometry->indexBuffer));

	//set misc
	geometry->num_vertices = (DWORD) num_vertices;
	geometry->num_primitives = (DWORD) num_indices / 3;
	geometry->texture = texture == NULL ? NULL : (Texture*) texture;

	return (Rocket::Core::CompiledGeometryHandle)geometry;
}

// Called by Rocket when it wants to render application-compiled geometry.
void RenderInterfaceDx11 :: RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{

	RocketDx11CompiledGeometry* _geometry = (RocketDx11CompiledGeometry*)geometry;

	//set states
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_context->OMSetBlendState(bs_normal, blendFactor, 0xffffffff);

	// set which texture to use
	if(_geometry->texture == NULL)
		_geometry->texture = defaultTex;
	
	_geometry->texture->useTexture();

	// Build and set the transform matrix.
	D3DXMatrixTranslation(&mat_world, translation.x, translation.y, 0);
	
	//==================================================================
	// HACK below: changing coordinate system from ScreenSpace to NDC:
	//    0%
	// 0% #---#
	//	  |   |
	//    #---# 100%
	//        100%
	// This should probably not have to be done. I'm not sure though.
	// I'll let this stay until I know for sure
	// - Mattias L

	D3DXMATRIX _mat_scale;
	D3DXMATRIX _mat_translate;

	D3DXMatrixIdentity(&_mat_scale); 
	D3DXMatrixIdentity(&_mat_translate);

	D3DXMatrixScaling(&_mat_scale , 2.0f/m_width, -2.0f/m_height, 1); 
	D3DXMatrixTranslation(&_mat_translate, -1, 1, 0);
	
	mat_world = mat_world * _mat_scale * _mat_translate;
	//==================================================================

	HR(fxVar_world->SetMatrix(&mat_world._11));

	m_context->IASetInputLayout(inputLayout);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VertexType);
	UINT offset = 0;
	m_context->IASetVertexBuffers(0, 1, &_geometry->vertexBuffer, &stride, &offset);
	m_context->IASetIndexBuffer(_geometry->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	HR(pass->Apply(passNr, m_context));

	// draw to the the back buffer
	m_context->DrawIndexed(_geometry->num_primitives*3, 0, 0);
}

// Called by Rocket when it wants to release application-compiled geometry.
void RenderInterfaceDx11 :: ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{

	RocketDx11CompiledGeometry* _geometry = (RocketDx11CompiledGeometry*) geometry;

	SAFE_RELEASE(_geometry->vertexBuffer);
	SAFE_RELEASE(_geometry->indexBuffer);

	//delete _geometry->texture;
	delete _geometry;
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void RenderInterfaceDx11 :: EnableScissorRegion(bool enable)
{
	if(enable == true)
		m_context->RSSetState(rs_scissorsOn);
	else
		m_context->RSSetState(rs_scissorsOff);
}

// Called by Rocket when it wants to change the scissor region.
void RenderInterfaceDx11 :: SetScissorRegion(int x, int y, int width, int height)
{
	D3D11_RECT scissor_rect;
	scissor_rect.left = x;
	scissor_rect.right = x + width;
	scissor_rect.top = y;
	scissor_rect.bottom = y + height;

	m_context->RSSetScissorRects(1, &scissor_rect);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.
bool RenderInterfaceDx11 :: LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (file_handle == NULL)
		return false;

	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool RenderInterfaceDx11 :: GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	Texture* _texture = new Texture;
	_texture->setDeviceAndFx(m_device, m_context, m_effect);

	_texture->setDynamic(source_dimensions.x, source_dimensions.y);
	_texture->setAllPixels(source);
	_texture->initTexture();

	// Set the handle on the Rocket texture structure.
	texture_handle = (Rocket::Core::TextureHandle)_texture;
	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void RenderInterfaceDx11 :: ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	Texture* _texture = ((Texture*) texture_handle);
	delete _texture;
}

// Returns the native horizontal texel offset for the renderer.
float RenderInterfaceDx11 :: GetHorizontalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

// Returns the native vertical texel offset for the renderer.
float RenderInterfaceDx11 :: GetVerticalTexelOffset()
{
	//return -0.5f;
	return 0.0f;
}

void RenderInterfaceDx11::defineInputElementDesc()
{
	// create the input element descriptions
	numElements = 3;
	layoutDesc = new D3D11_INPUT_ELEMENT_DESC[numElements];

	layoutDesc[0].SemanticName = "POSITION";
	layoutDesc[0].SemanticIndex = 0;
	layoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layoutDesc[0].InputSlot = 0;
	layoutDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[0].InstanceDataStepRate = 0;

	//use normal instead of color
	layoutDesc[1].SemanticName = "COLOR";
	layoutDesc[1].SemanticIndex = 0;
	layoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layoutDesc[1].InputSlot = 0;
	layoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[1].InstanceDataStepRate = 0;

	//texture
	layoutDesc[2].SemanticName = "TEXCOORD";
	layoutDesc[2].SemanticIndex = 0;
	layoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	layoutDesc[2].InputSlot = 0;
	layoutDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[2].InstanceDataStepRate = 0;
}

void RenderInterfaceDx11::initPipeline()
{
	technique = m_effect->GetTechniqueByIndex(techNr);
	pass = technique->GetPassByIndex(passNr);
	HR(pass->GetDesc(&PassDesc));

	//layout - input layout description is set in subclass's constructor
	HR(m_device->CreateInputLayout(layoutDesc, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &inputLayout));
}

void RenderInterfaceDx11::initFxVars()
{
	fxVar_world = m_effect->GetVariableByName("mat_world")->AsMatrix();
}

void RenderInterfaceDx11::defineBlendState()
{
	D3D11_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = false;
	//blendDesc.BlendEnable[0] = true; old dx10
	blendDesc.IndependentBlendEnable = false; // Changed in dx11 conversion. Should be false?
	// RenderTarget[0] new for dx11
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(m_device->CreateBlendState(&blendDesc, &bs_normal));
}

void RenderInterfaceDx11::initDefaultTex()
{
	defaultTex = new Texture();
	defaultTex->setDeviceAndFx( m_device, m_context, m_effect );
	defaultTex->setDynamic( 1, 1 );
	byte pix[] =  {128, 128, 128, 128};
	defaultTex->setPixel( pix, 0, 0 );
	defaultTex->initTexture();
}

void RenderInterfaceDx11::initDevice()
{
	DXGI_SWAP_CHAIN_DESC scd;    //Swap Chain Device
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));    //Clear before use

	scd.BufferCount = 1;    //Two buffers
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = m_hWnd;
	scd.SampleDesc.Count = 1;    //Level of multi sampling, 1 = No
	scd.SampleDesc.Quality = 0;    //Quality of multi-sampling
	scd.Windowed = TRUE;

	// Feature Levels
	HRESULT hr = S_OK;
	D3D_DRIVER_TYPE driverType;
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&scd,
			&m_swapchain,
			&m_device,
			&initiatedFeatureLevel,
			&m_context);

		if( SUCCEEDED( hr ) )
		{
			char title[256];
			sprintf_s(
				title,
				sizeof(title),
				"BTH - Direct3D 11.0 Template | Direct3D 11.0 device initiated with Direct3D %s feature level",
				FeatureLevelToString(initiatedFeatureLevel)
				);
			SetWindowTextA(m_hWnd, title); //A for ascii. Not from stefan

			break;
		}
	}
	if( FAILED(hr) )
		return;

	// create a texture for the depth buffer
	D3D11_TEXTURE2D_DESC zbd;
	ZeroMemory(&zbd, sizeof(zbd));
	zbd.Width = m_width;    // set the width of the depth buffer
	zbd.Height = m_height;    // set the height of the depth buffer
	zbd.ArraySize = 1;    // we are only creating one texture
	zbd.SampleDesc.Count = 1;    // no multi-sampling
	zbd.Format = DXGI_FORMAT_D32_FLOAT;    // one 32-bit float per pixel
	zbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;    // texture is to be used as a depth buffer
	ID3D11Texture2D* pDepthBuffer;
	HR(m_device->CreateTexture2D(&zbd, NULL, &pDepthBuffer));    // create the texture

	// create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;    // one 32-bit float per pixel
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;    // depth buffer is a 2D texture
	HR(m_device->CreateDepthStencilView(pDepthBuffer, &dsvd, &m_dsv));    // create the depth buffer
	pDepthBuffer->Release();    // release the texture once the depth buffer is made

	//Get the address of the back buffer and use it to create the render target
	ID3D11Texture2D* backBufferPtr;
	m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	HR(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_rtv));
	backBufferPtr->Release();   //backBufferPtr no longer needed, release to free RAM


	ZeroMemory(&m_viewport, sizeof(m_viewport));
	m_viewport.TopLeftX = 0;    
	m_viewport.TopLeftY = 0;    
	m_viewport.Width = m_width;    
	m_viewport.Height = m_height;
	m_viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	m_viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0
}

void RenderInterfaceDx11::initEffect()
{
	//Debug
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	//Send blob to gather debug data
	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	HRESULT hr = 0;

	hr = D3DX11CompileFromFile(
		L"../shaders/effect.fx",
		NULL,
		NULL,
		"",
		"fx_5_0",
		shaderFlags,
		NULL,
		NULL,
		&pBlobEffect,
		&pBlobErrors,
		NULL
		);


	// If compilation of the .fx-file fails. Present compilation errors to the user
	char* l_pError = NULL;
	if( FAILED(hr) )
	{
		char msg[20000];
		strcpy_s(msg, sizeof(msg), (char*)pBlobErrors->GetBufferPointer());
		OutputDebugStringA(msg); // A as in ASCII. dx11 conversion
		MessageBoxA(GetDesktopWindow(), msg, "Effect compilation error", MB_OK | MB_ICONERROR); // A as in ASCII. dx11 conversion
		return;
	}

	if(FAILED(hr = D3DX11CreateEffectFromMemory(
		pBlobEffect->GetBufferPointer(),
		pBlobEffect->GetBufferSize(),
		shaderFlags,
		m_device,
		&m_effect
		)))
	{
		MessageBoxA(0, "Cannot create effect from memory.", "D3DX11CreateEffectFromMemory error", MB_OK | MB_ICONERROR); // A as in ASCII. dx11 conversion
		return;
	}

	//hr = D3DX11CompileFromFile(L"../shaders/effect.fx", 0, 0, "fx_4_0", shaderFlags, 0,
	//	m_device, 0, 0, &m_effect, &compilationErrors, 0);
	// the FAILED() macro modifies the hr, this copy is used with the HR() macro
}

void RenderInterfaceDx11::beginFrame()
{
	m_context->RSSetViewports(1, &m_viewport);    //Set the view port
	m_context->ClearRenderTargetView( m_rtv, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f) );
	m_context->ClearDepthStencilView( m_dsv, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//#1F7116
	// clear the window to a deep blue
	//Set the render target as the back buffer
	//m_device->OMSetRenderTargets(1, &m_rtv, m_dsv);
	// HACK: Use no depth buffer = text will not fail depth test
	m_context->OMSetRenderTargets(1, &m_rtv, NULL);

	// reset states
	m_context->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_context->OMSetBlendState( 0, blendFactors, 0xffffffff );
}

void RenderInterfaceDx11::presentFrame()
{
	// display the rendered frame
	HR( m_swapchain->Present(0, 0) );
}