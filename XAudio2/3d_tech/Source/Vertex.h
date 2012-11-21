#ifndef VERTEX_H
#define VERTEX_H

#include <D3D10.h>

struct Vertex
{
	Vertex(){}
	Vertex( float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
		: pos(x,y,z),normal(nx,ny,nz),texC(u,v){}
	Vertex(D3DXVECTOR3 pPos,  D3DXVECTOR2 pTexC, D3DXVECTOR3 pNormal)
	{
		pos = pPos;
		normal = pNormal;
		texC = pTexC;
	}
	Vertex(D3DXVECTOR3 pPos)
	{
		pos = pPos;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

#endif