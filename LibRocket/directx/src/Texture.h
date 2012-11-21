#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "Utils.h"

using namespace std;

class Texture
{
private:
	ID3D10Device* device;
	ID3D10Effect* effect;
	D3D10_TEXTURE2D_DESC desc_texture;
	ID3D10Texture2D* tex_texture;
	ID3D10ShaderResourceView* srv_texture;
	ID3D10EffectShaderResourceVariable* fxVar_texture;
	string resourceFolder;
	string texFileName;
	string texVarName;

public:
	Texture();
	~Texture();
	void clear();
	void setDeviceAndFx(ID3D10Device* _device, ID3D10Effect* _effect);
	void setTexture(string _resourceFolder, string _textureName);
	void setShaderTargets(string _texVarName);
	void setDynamic(int _width, int _height);
	void setPixel(const byte* _source, int _x, int _y);
	void setAllPixels(const byte* _source);
	void initTexture();
	void useTexture();
};

#endif //TEXTURE_H