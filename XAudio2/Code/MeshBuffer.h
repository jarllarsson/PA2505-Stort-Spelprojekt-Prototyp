/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/5/2012 												*/
/************************************************************************/

#ifndef MESHBUFFER_H__
#define MESHBUFFER_H__

#include "stdafx.h"

struct MeshBuffer
{
	ID3D11Buffer	*mVertexBuffer, 
					*mIndiciesBuffer;
	DWORD			mIndices;

	MeshBuffer()
	{
		mVertexBuffer	= NULL;
		mIndiciesBuffer = NULL;
		mIndices		= 0;
	}
};


#endif // MESHBUFFER_H__