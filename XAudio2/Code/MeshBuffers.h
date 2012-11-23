/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/20/2012 												*/
/************************************************************************/

#ifndef MESHBUFFERS_H
#define MESHBUFFERS_H
#include "Buffer.h"

struct MeshBuffers
{
	Buffer	*vertexBuffer,
			*indexBuffer;
};

#endif // MESHBUFFERS_H