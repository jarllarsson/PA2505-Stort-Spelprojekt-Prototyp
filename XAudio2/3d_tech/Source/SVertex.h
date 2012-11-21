#ifndef SVERTEX_H
#define SVERTEX_H

#include <D3D10.h>

struct SVertex
{
	D3DXVECTOR3 Position; 
	D3DXCOLOR Color;

/*	SVertex()
	{
		Color = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	}*/
};

#endif