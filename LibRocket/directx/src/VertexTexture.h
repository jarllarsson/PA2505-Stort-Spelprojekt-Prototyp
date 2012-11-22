#include <D3D10.h>
#include <D3DX10.h>

#ifndef VERTEXTEXTURE_H
#define VERTEXTEXTURE_H
struct VertexTexture 
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 texCord;
	D3DXVECTOR3 Tangent;
};
#endif //VERTEXTEXTURE_H

