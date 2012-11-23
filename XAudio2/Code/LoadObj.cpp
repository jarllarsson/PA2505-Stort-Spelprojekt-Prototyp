#include "LoadObj.h"

LoadObj::LoadObj()
{
	reset();
}

LoadObj::~LoadObj()
{

}

MeshData* LoadObj::readObjFile(std::string pObj)
{
	bool first = true;
	fstream f(pObj.c_str());
	string tmp;

	if(f)
	{
		while(f.eof() != true)
		{
			f >> tmp;

			if(tmp == "v")
				readVertices(f);
			if(tmp == "vt")
				readTexC(f);
			if(tmp == "vn")
				readNormal(f);
			if(tmp == "f")	
				readFaces(f);
			if(tmp == "mtllib")
				readMtl(f);
			if(tmp == "g")
				readGroup(f);
			if(f.eof())
				break;
		}
	}

	MeshData* data = new MeshData();

	data->position			= position;
	data->textureCoordinate	= textureCoordinate;
	data->normal			= normal;

	data->vertices			= vertices;
	data->indices			= indices;

	reset();

	return data;
}

void LoadObj::readVertices(fstream &f)
{
	float x, y, z;
	f >> x >> y >> z;
	position.push_back(D3DXVECTOR3(x,y,z));
}

void LoadObj::readTexC(fstream& f)
{
	float u, v;
	f >> u >> v;
	textureCoordinate.push_back(D3DXVECTOR2(u,v));
}

void LoadObj::readNormal(fstream& f)
{
	float nx, ny, nz;
	f >> nx >> ny >> nz;
	normal.push_back(D3DXVECTOR3(nx,ny,nz));
}

void LoadObj::readFaces(fstream& f)
{
	for(int i= 0;i<3;i++)
	{
		char s;
		DWORD lPos, lTexC, lNormal;
		f >> lPos >> s >> lTexC >> s >> lNormal;

		Vertex temp;

		temp.pos = position.at(lPos-1);
		temp.normal = normal.at(lNormal-1);
		temp.texC = textureCoordinate.at(lTexC-1);

		vertices.push_back(temp);
		indices.push_back(vertices.size()-1);
	}	
}
void LoadObj::readMtl(fstream& f)
{
	string file;
	materialPath = "Mesh/";

	f >> file;

	materialPath += file;
}

void LoadObj::readGroup(fstream& f)
{
	f >> group;
}

void LoadObj::reset()
{
	position.clear();
	textureCoordinate.clear();
	normal.clear();
	vertices.clear();
	indices.clear();

	position.reserve(10000);
	textureCoordinate.reserve(10000);
	normal.reserve(10000);

	vertices.reserve(10000);
	indices.reserve(10000);
}