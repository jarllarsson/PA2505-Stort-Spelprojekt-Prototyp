#include "Heightmap.h"
#include "Camera.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

Heightmap::Heightmap() : Object(D3DXVECTOR3(0,0,0))
{
	mNumRows		= 0;
	mNumCols		= 0;
	mNumVertices	= 0;
	mNumFaces		= 0;
	mDevice			= 0;
	indexBuffer		= NULL;
	vertextBuffer	= NULL;
	mShader			= NULL;
}

Heightmap::~Heightmap()
{
	SAFE_RELEASE(vertextBuffer);
	SAFE_RELEASE(indexBuffer);
}

HRESULT Heightmap::init(ID3D11Device* device,ID3D11DeviceContext* deviceContext,Shader* shader,char* heightMapPath, DWORD m, DWORD n, float dx)
{
	HRESULT hr = S_OK;
	mDevice = device;
	mDeviceContext = deviceContext;
	mShader = shader;

	hr = D3DX11CreateShaderResourceViewFromFile(device,"../Textures/ggrass.dds",0,0,&mAlbedoMap,0);

	mNumRows  = m;
	mNumCols  = n;

	mCellSpacing = dx;

	mNumVertices = mNumRows*mNumCols;
	mNumFaces    = (mNumRows-1)*(mNumCols-1)*2;

	mHeightOffset = -1.5;
	mHeightScale = 0.25;

	loadHeightMap(heightMapPath);
	smooth();

	Vertex* vertexArray = new Vertex[mNumVertices];

	float halfWidth = (mNumCols-1)*mCellSpacing*0.5f;
	float halfDepth = (mNumRows-1)*mCellSpacing*0.5f;
	
	float du = 1.0f / (mNumCols-1);
	float dv = 1.0f / (mNumRows-1);

	for(DWORD i = 0; i < mNumRows; ++i)
	{
		float z = halfDepth - i*dx;
		for(DWORD j = 0; j < mNumCols; ++j)
		{
			int vIndex = i*mNumCols+j;
			vertexArray[vIndex].pos		= D3DXVECTOR3(-halfWidth + j*dx, mHeightmap[i*mNumCols+j]-5, z);
			vertexArray[vIndex].texC	= D3DXVECTOR2(j*du, i*dv);
			vertexArray[vIndex].normal	= D3DXVECTOR3();
		}
	}

	CalculateNormals(vertexArray);
	InitBuffers(vertexArray);

	delete vertexArray;
	return hr;
}
void Heightmap::CalculateNormals(Vertex *vertexArray)
{
	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*mCellSpacing);
	float invTwoDZ = 1.0f / (2.0f*mCellSpacing);
	for(UINT i = 2; i < mNumRows-1; ++i)
	{
		for(UINT j = 2; j < mNumCols-1; ++j)
		{
			float t = mHeightmap[(i-1)*mNumCols + j];
			float b = mHeightmap[(i+1)*mNumCols + j];
			float l = mHeightmap[i*mNumCols + j - 1];
			float r = mHeightmap[i*mNumCols + j + 1];

			D3DXVECTOR3 tanZ(0.0f, (t-b)*invTwoDZ, 1.0f);
			D3DXVECTOR3 tanX(1.0f, (r-l)*invTwoDX, 0.0f);

			D3DXVECTOR3 N;
			D3DXVec3Cross(&N, &tanZ, &tanX);
			D3DXVec3Normalize(&N, &N);

			vertexArray[i*mNumCols+j].normal = N;
		}
	}
}
HRESULT Heightmap::InitBuffers(Vertex* vertexArray)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertexArray[0];
	mDevice->CreateBuffer(&vbd, &vinitData, &vertextBuffer);

	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<DWORD> indices(mNumFaces*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(DWORD i = 0; i < mNumRows-1; ++i)
	{
		for(DWORD j = 0; j < mNumCols-1; ++j)
		{
			indices[k]   = i*mNumCols+j;
			indices[k+1] = i*mNumCols+j+1;
			indices[k+2] = (i+1)*mNumCols+j;

			indices[k+3] = (i+1)*mNumCols+j;
			indices[k+4] = i*mNumCols+j+1;
			indices[k+5] = (i+1)*mNumCols+j+1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	mDevice->CreateBuffer(&ibd, &iinitData, &indexBuffer);

	return S_OK;
}
HRESULT Heightmap::Render(D3DXMATRIX viewProjection)
{	

	D3DXMATRIX landTexMtx;
	D3DXMatrixScaling(&landTexMtx, 50.0f, 50.0f, 1);

	D3DXMATRIX worldViewProjection, world;
	world = getFinalMatrix();
	worldViewProjection = world * viewProjection;

	mShader->SetMatrix("gWorld",world);
	mShader->SetMatrix("gWVP", worldViewProjection);
	mShader->SetMatrix("texMat",landTexMtx);
	mShader->SetResource("gAlbedoMap",mAlbedoMap);
	mShader->Apply(0);
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mDeviceContext->IASetVertexBuffers(0, 1, &vertextBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->DrawIndexed(mNumFaces*3,0,0);

	return S_OK;
}
float Heightmap::width()const
{
	return (mNumCols-1)*mCellSpacing;
}
float Heightmap::depth()const
{
	return (mNumRows-1)*mCellSpacing;
}
bool Heightmap::inBounds(UINT i, UINT j)
{
	return i >= 0 && i < mNumRows && j >= 0 && j < mNumCols;
}
float Heightmap::average(UINT i, UINT j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for(UINT m = i-1; m <= i+1 ; ++m)
	{
		for(UINT n = j-1; n<= j+1; ++n)
		{
			if(inBounds(m,n))
			{
				avg += mHeightmap[m*mNumCols + n];
				num += 1.0f;
			}
		}
	}
	return avg/num;
}
void Heightmap::smooth()
{
	vector<float> dest( mHeightmap.size(),0 );

	for(UINT i = 0; i < mNumRows; ++i)
	{
		for(UINT j = 0; j<mNumCols; ++j)
		{
			dest[i*mNumCols+j] = average(i,j);
		}
	}

	//Replace the old heightmap with the filtered one.
	mHeightmap = dest;
}
void Heightmap::loadHeightMap(char* filePath)
{
	//A height for each vertex
	vector<unsigned char> in(mNumRows*mNumCols);

	//Open the file
	ifstream inFile;
	inFile.open(filePath,ios_base::binary);

	if(inFile)
	{
		//Read theee rawwww bytes
		inFile.read((char*)&in[0], (streamsize)in.size());

		//done with fillleee
		inFile.close();
	}
	
	//copy the array data into a float array and scale and offset the heights.
	mHeightmap.resize(mNumRows*mNumCols,0);
	
	for(unsigned int i = 0;i < mNumRows*mNumCols; ++i)
	{
		mHeightmap[i] = (float)in[i] * mHeightScale + mHeightOffset;
	}
}
HRESULT Heightmap::Update(float deltaTime)
{
	return S_OK;
}
float Heightmap::getHeight(float x,float z)
{
	float c  = (x+0.5f*width())/ mCellSpacing;
	float d  = (z-0.5f*depth())/ -mCellSpacing;

	int row = (int)floorf(d);
	int col = (int)floorf(c);

	float A,B,C,D;
	A = mHeightmap[row*mNumCols+col];
	B = mHeightmap[row*mNumCols+col+1];
	C = mHeightmap[(row+1)*mNumCols + col];
	D = mHeightmap[(row+1)*mNumCols + col + 1];

	float s = c -(float)col;
	float t = d - (float)row;

	if(s+t<=1.0f)
	{
		float uy = B-A;
		float vy = C-A;
		return A + s*uy+t*vy;
	}
	else
	{
		float uy = C-D;
		float vy = B-D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}


float Heightmap::getSize()const 
{
	return (float)(mNumRows/2)-2;
}