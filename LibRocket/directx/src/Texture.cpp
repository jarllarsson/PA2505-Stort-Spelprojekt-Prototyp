#include "Texture.h"

Texture::Texture()
{
	device			= NULL;
	effect			= NULL;
	tex_texture		= NULL;
	srv_texture		= NULL;
	fxVar_texture	= NULL;
	texFileName		= "NULL";
	texVarName		= "tex_default";
}

Texture::~Texture()
{
	clear();
}

void Texture::clear()
{
	SAFE_RELEASE(tex_texture);
	SAFE_RELEASE(srv_texture);
	//SAFE_RELEASE(fxVar_texture);
}

void Texture::setDeviceAndFx( ID3D11Device* p_device, ID3D11DeviceContext* p_context, ID3DX11Effect* p_effect )
{
	device = p_device;
	m_context = p_context;
	effect = p_effect;
}

void Texture::setTexture(string _resourceFolder, string _texFileName)
{
	resourceFolder = _resourceFolder;
	texFileName = _texFileName;
}

void Texture::setShaderTargets(string _texVarName)
{
	texVarName = _texVarName;
}

void Texture::setDynamic(int _width, int _height)
{
	clear();

	ZeroMemory(&desc_texture, sizeof(desc_texture));

	desc_texture.Width = _width;
	desc_texture.Height = _height;
	desc_texture.MipLevels = 1;
	desc_texture.ArraySize = 1;
	desc_texture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc_texture.SampleDesc.Count = 1;
	desc_texture.Usage = D3D11_USAGE_DYNAMIC;
	desc_texture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc_texture.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HR(device->CreateTexture2D(&desc_texture, NULL, &tex_texture));
}

void Texture::setPixel(const byte* _source, int _x, int _y)
{
	//D3D10_MAPPED_TEXTURE2D mappedTex;
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	//tex_texture->Map( D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex );
	m_context->Map( tex_texture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex );


	UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	UINT rowStart = _x * mappedTex.RowPitch;
	UINT colStart = _y * 4;

	pTexels[rowStart + colStart + 0] = _source[rowStart + colStart + 0]; // Red
	pTexels[rowStart + colStart + 1] = _source[rowStart + colStart + 1]; // Green
	pTexels[rowStart + colStart + 2] = _source[rowStart + colStart + 2];  // Blue
	pTexels[rowStart + colStart + 3] = _source[rowStart + colStart + 3];  // Alpha

	//tex_texture->Unmap( D3D11CalcSubresource(0, 0, 1) );
	m_context->Unmap( tex_texture, D3D11CalcSubresource(0, 0, 1) );
}

void Texture::setAllPixels(const byte* _source)
{
	//D3D11_MAPPED_TEXTURE2D mappedTex;	
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	m_context->Map( tex_texture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex );

	UCHAR* texels = (UCHAR*)mappedTex.pData;
	for( UINT row = 0; row < desc_texture.Height; row++ )
	{
		UINT rowStart = row * mappedTex.RowPitch;
		for( UINT col = 0; col < desc_texture.Width; col++ )
		{
			UINT colStart = col * 4;

			texels[rowStart + colStart + 0] = _source[rowStart + colStart + 0]; //red		1
			texels[rowStart + colStart + 1] = _source[rowStart + colStart + 1]; //green		0
			texels[rowStart + colStart + 2] = _source[rowStart + colStart + 2]; //blue		3
			texels[rowStart + colStart + 3] = _source[rowStart + colStart + 3]; //alpha		2
		}
	}

	m_context->Unmap( tex_texture, D3D11CalcSubresource(0, 0, 1) );
}

void Texture::initTexture()
{
	if(tex_texture != NULL)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc_texture.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc_texture.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = desc_texture.MipLevels -1;

		HR(device->CreateShaderResourceView(tex_texture, &srvDesc, &srv_texture));
	}
	else
	{
		wstring wstrTemp(texFileName.length(), L' ');
		copy(texFileName.begin(), texFileName.end(), wstrTemp.begin());

		HR(D3DX11CreateShaderResourceViewFromFile(device, wstrTemp.c_str(), NULL, NULL, &srv_texture, NULL));
	}
}

void Texture::useTexture()
{
	wstring wstrTemp(texVarName.length(), L' ');
	copy(texFileName.begin(), texFileName.end(), wstrTemp.begin());

	fxVar_texture = effect->GetVariableByName(texVarName.c_str())->AsShaderResource();
	HR(fxVar_texture->SetResource(srv_texture));
}