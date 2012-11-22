#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3D11.h>
#include <d3dx11.h>
#include "Utils.h"
#include <d3dx11effect.h>

using namespace std;

class Texture
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* m_context;
	ID3DX11Effect* effect;
	D3D11_TEXTURE2D_DESC desc_texture;
	ID3D11Texture2D* tex_texture;
	ID3D11ShaderResourceView* srv_texture;
	ID3DX11EffectShaderResourceVariable* fxVar_texture;
	string resourceFolder;
	string texFileName;
	string texVarName;

public:
	Texture();
	~Texture();
	void clear();
	void setDeviceAndFx( ID3D11Device* p_device, ID3D11DeviceContext* p_context, ID3DX11Effect* p_effect );
	void setTexture(string _resourceFolder, string _textureName);
	void setShaderTargets(string _texVarName);
	void setDynamic(int _width, int _height);
	void setPixel(const byte* _source, int _x, int _y);
	void setAllPixels(const byte* _source);
	void initTexture();
	void useTexture();
};

#endif //TEXTURE_H