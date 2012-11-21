#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <d3dx10.h>
#include "Vertex.h"
#include <fstream>
#include "SVertex.h"
#include <vector>
#include <string>
#include "Object.h"

using namespace std;

class Heightmap : public Object
{
public:
	Heightmap(string path);
	Heightmap();
	~Heightmap(void);

	float getHeight(float x, float z)const;
	void init(ID3D10Device* device, DWORD m, DWORD n, float dx);
	void update(float deltaTime);
	void draw();
	void loadHeightMap();
	bool inBounds(UINT i, UINT j);
	float average(UINT i, UINT j);
	void smooth();
	float getHeight(float x, float z);
	float width()const;
	float depth()const;
	float getSize()const;
private:
	DWORD mNumRows;
	DWORD mNumCols;

	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* device;
	ID3D10Buffer* mTerrainVB;
	ID3D10Buffer* mTerrainIB;

	std::vector<float> mHeightmap;

	float mHeightScale;
	float mHeightOffset;
	float mCellSpacing;


	ID3D10Effect* effect;
	ID3D10EffectTechnique* effectTechnique;
	ID3D10EffectPass* effectPass;
	ID3D10InputLayout* inputLayout;
	ID3D10EffectMatrixVariable* worldMatrix;    // the pointer to the effect variable interface
	
	void init_pipeline();

};

#endif