#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "stdafx.h"
#include "Object.h"
#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"

using namespace std;

class Heightmap : public Object
{
private:
	DWORD				mNumRows,
						mNumCols,
						mNumVertices,
						mNumFaces;

	ID3D11Device		*mDevice;
	ID3D11DeviceContext	*mDeviceContext;
	ID3D11ShaderResourceView* mAlbedoMap;

	ID3D11Buffer		*vertextBuffer,
						*indexBuffer;

	Shader				*mShader;
	
	vector<float> mHeightmap;
	float	mHeightScale,
			mHeightOffset,
			mCellSpacing;
private:
	HRESULT InitBuffers(Vertex* vertexArray);
	float	average(UINT i, UINT j);
	void	smooth();
	void	CalculateNormals(Vertex *vertexArray);
public:
	Heightmap();
	~Heightmap(void);
	HRESULT init(ID3D11Device* device,ID3D11DeviceContext* deviceContext,Shader* shader,char* heightMapPath, DWORD m, DWORD n, float dx);
	HRESULT Update(float deltaTime);
	HRESULT Render(D3DXMATRIX viewProjection);
	void	loadHeightMap(char* filePath);
	bool	inBounds(UINT i, UINT j);
	float	getHeight(float x, float z);
	float	width()const;
	float	depth()const;
	float	getSize()const;
};

#endif