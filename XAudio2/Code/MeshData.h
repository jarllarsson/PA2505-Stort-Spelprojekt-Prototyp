/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/10/2012 												*/
/************************************************************************/

#ifndef MESHDATA_H
#define MESHDATA_H

#include "Vertex.h"
#include <vector>

struct MeshData
{
	std::vector<D3DXVECTOR3>	position;
	std::vector<D3DXVECTOR2>	textureCoordinate;
	std::vector<D3DXVECTOR3>	normal;
	
	std::vector<Vertex>			vertices;
	std::vector<DWORD>			indices;
};

#endif // MESHDATA_H