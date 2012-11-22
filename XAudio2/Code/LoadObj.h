/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/5/2012 												*/
/************************************************************************/

#ifndef LOADOBJ_H__
#define LOADOBJ_H__

#include <string>
#include <fstream>
#include <vector>
#include "stdafx.h"
#include "Vertex.h"
#include "MeshData.h"

using namespace std;

class LoadObj
{
private:
	vector<D3DXVECTOR3>		position;
	vector<D3DXVECTOR2>		textureCoordinate;
	vector<D3DXVECTOR3>		normal;

	vector<Vertex>			vertices;
	vector<DWORD>			indices;

	string					materialPath;
	string					group;
private:
	void	readVertices(fstream  &f);
	void	readTexC(fstream& f);
	void	readNormal(fstream& f);
	void	readFaces(fstream& f);
	void	readMtl(fstream& f);
	void	readGroup(fstream& f);
	void	reset();
public:
	LoadObj();
	~LoadObj();
	MeshData*	readObjFile(string pObj);
};
#endif // LOADOBJ_H__
