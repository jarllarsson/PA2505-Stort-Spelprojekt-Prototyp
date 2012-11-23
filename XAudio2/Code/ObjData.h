/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/21/2012 												*/
/************************************************************************/

#ifndef OBJDATA_H
#define OBJDATA_H

#include "MeshBuffers.h"
#include "MinMax.h"

struct ObjData
{
	MeshBuffers* data;
	MinMax		minMax;
};

#endif // OBJDATA_H