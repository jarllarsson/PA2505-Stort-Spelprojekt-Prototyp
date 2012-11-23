/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/8/2012 												*/
/************************************************************************/

#ifndef PVERTEX_H__
#define PVERTEX_H__

#include "stdafx.h"

struct PVertex
{
	D3DXVECTOR3 pos;
	float		pad;

	PVertex(D3DXVECTOR3 p_pos)
	{
		pos = p_pos;
	}
};

#endif // PVERTEX_H__
