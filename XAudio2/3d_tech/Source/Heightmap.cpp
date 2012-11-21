#include "Heightmap.h"
#include "Camera.h"
#include <windows.h>
#include <tchar.h>

Heightmap::Heightmap() : Object(D3DXVECTOR3(0,0,0))
{
	mNumRows = 0;
	mNumCols = 0;
	mNumVertices = 0;
	mNumFaces = 0;
	device = 0;
	mTerrainVB = 0;
	mTerrainIB = 0;
}

Heightmap::~Heightmap()
{
	mTerrainVB->Release();
	mTerrainIB->Release();
}

float Heightmap::getHeight(float x, float z)const
{
	return 0.3f*( z*sinf(0.1f*x) + x*cosf(0.1f*z) );
}

void Heightmap::init(ID3D10Device* device, DWORD m, DWORD n, float dx)
{
	this->device = device;

	mNumRows  = m;
	mNumCols  = n;

	mCellSpacing = 1.0f;

	mNumVertices = m*n;
	mNumFaces    = (m-1)*(n-1)*2;

	//Statiska variablar
	this->mHeightOffset = -1.5;
	this->mHeightScale = 0.25;
	//--------------------

	this->loadHeightMap();
	this->smooth();

	// Create the geometry and fill the vertex buffer. 

	std::vector<SVertex> vertices(mNumVertices);
	float halfWidth = (n-1)*dx*0.5f;
	float halfDepth = (m-1)*dx*0.5f;
	
	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	const D3DXCOLOR col = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	SVertex lTemp;
	lTemp.Color = col;
	/*
	DWORD StartTime, ElapsedTime;
	_asm {
		RDTSC
		mov StartTime, eax
	}
	*/
	for(DWORD i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dx;
		for(DWORD j = 0; j < n; j++)
		{
			// Graph of this function looks like a mountain range.
			//float y = getHeight(x,z);
			//Positionen för varje punkt
			int vIndex = i*n+j;
			vertices[vIndex].Position = D3DXVECTOR3(-halfWidth + j*dx, mHeightmap[i*this->mNumCols+j]-5, z);
			vertices[vIndex].Color = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
			
			/*
			lTemp.Position = D3DXVECTOR3(-halfWidth + (j+1)*dx, mHeightmap[i*this->mNumCols+j+1], z);
			vertices[i*n+j+1] = lTemp;
			lTemp.Position = D3DXVECTOR3(-halfWidth + (j+2)*dx, mHeightmap[i*this->mNumCols+j+2], z);
			vertices[i*n+j+2] = lTemp;
			lTemp.Position = D3DXVECTOR3(-halfWidth + (j+3)*dx, mHeightmap[i*this->mNumCols+j+3], z);
			vertices[i*n+j+3] = lTemp;
			*/

			/* Will be needed later in project now I just use color and position
			//Normalen för varje punkt
			vertices[i*n+j].normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
			//Textur kordinaten för varje punkt
			vertices[i*n+j].texC.x = j*du;
			vertices[i*n+j].texC.y = i*dv;
			*/

			/*
			//Justerar hur marken förhåller sig till ljuset
			vertices[i*n+j].diffuse = BEACH_SAND;
			vertices[i*n+j].spec = D3DXCOLOR(0.2f, 0.2f, 0.2f, 32.0f);
			*/
		}
	}
	/*_asm {
		RDTSC
		sub eax, StartTime
		mov ElapsedTime, eax
	}
	TCHAR str[255];
	_stprintf(str, _T("Time in CPU clocks %d"), ElapsedTime);
	MessageBox(0, str, 0, MB_OK);
	*/

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SVertex) * mNumVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	device->CreateBuffer(&vbd, &vinitData, &mTerrainVB);

	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<DWORD> indices(mNumFaces*3); // 3 indices per face

	// Iterate over each quad and compute indices.
	int k = 0;
	for(DWORD i = 0; i < m-1; ++i)
	{
		for(DWORD j = 0; j < n-1; ++j)
		{
			indices[k]   = i*n+j;
			indices[k+1] = i*n+j+1;
			indices[k+2] = (i+1)*n+j;

			indices[k+3] = (i+1)*n+j;
			indices[k+4] = i*n+j+1;
			indices[k+5] = (i+1)*n+j+1;

			k += 6; // next quad
		}
	}

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&ibd, &iinitData, &mTerrainIB);

	init_pipeline();
	//init_triangle();
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
	return 
		i >= 0 && i < this->mNumRows && j >= 0 && j < this->mNumCols;
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
				avg += mHeightmap[m*this->mNumCols + n];
				num += 1.0f;
			}
		}
	}
	return avg/num;
}
void Heightmap::smooth()
{
	std::vector<float> dest( this->mHeightmap.size(),0 );

	for(UINT i = 0; i < this->mNumRows; ++i)
	{
		for(UINT j = 0; j<this->mNumCols; ++j)
		{
			dest[i*this->mNumCols+j] = average(i,j);
		}
	}

	//Replace the old heightmap with the filtered one.
	mHeightmap = dest;
}
void Heightmap::loadHeightMap()
{
	//A height for each vertex
	std::vector<unsigned char> in(this->mNumRows*this->mNumCols);

	//Open the file
	std::ifstream inFile;
	inFile.open("Terrain//height512x512.raw",std::ios_base::binary);

	if(inFile)
	{
		//Read theee rawwww bytes
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		//done with fillleee
		inFile.close();
	}
	
	//copy the array data into a float array and scale and offset the heights.
	mHeightmap.resize(mNumRows*mNumCols,0);
	
	for(unsigned int i = 0;i < this->mNumRows*this->mNumCols; ++i)
	{
		mHeightmap[i] = (float)in[i] * this->mHeightScale + this->mHeightOffset;
	}
}
void Heightmap::update(float deltaTime)
{
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
void Heightmap::draw()
{	worldMatrix->SetMatrix((float*)&getFinalMatrix());

	effectPass->Apply(0);
	
	UINT stride = sizeof(SVertex);
	UINT offset = 0;

	device->IASetInputLayout(inputLayout);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	device->IASetVertexBuffers(0, 1, &mTerrainVB, &stride, &offset);
	device->IASetIndexBuffer(mTerrainIB, DXGI_FORMAT_R32_UINT, 0);
	device->DrawIndexed(mNumFaces*3, 0, 0);
}

float Heightmap::getSize()const 
{
	return (float)(mNumRows/2)-2;
}

void Heightmap::init_pipeline()
{
	D3D10_PASS_DESC passDescritption;
	ID3D10Blob *effectBlob = 0, *errorsBlob = 0;

	// load the effect file
	HRESULT hr = D3DX10CompileFromFile(
		L"FX//effect.fx", 0, 0, 0,
		"fx_4_0", 0, 0, 0,
		&effectBlob, &errorsBlob, &hr);

	if(errorsBlob)
	{
		MessageBoxA(0,(char*)errorsBlob->GetBufferPointer(),0,0);
		errorsBlob->Release();
	}

	D3DX10CreateEffectFromMemory( 
		effectBlob->GetBufferPointer(),
		effectBlob->GetBufferSize(),
		0,
		0,
		0,
		0,
		0,
		0,
		device,
		0,
		0,
		&effect,
		&errorsBlob,
		&hr);

	// get the technique and the pass
	effectTechnique = effect->GetTechniqueByIndex(0);
	effectPass = effectTechnique->GetPassByIndex(0);
	effectPass->GetDesc(&passDescritption);

	// get the TimeElapsed effect variable
	worldMatrix = effect->GetVariableByName("transform")->AsMatrix();

	// create the input element descriptions
	D3D10_INPUT_ELEMENT_DESC Layout[] =
	{
		// first input element: position
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT,
		D3D10_INPUT_PER_VERTEX_DATA, 0},

		// second input element: color
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT,
		D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// use the input element descriptions to create the input layout
	device->CreateInputLayout(Layout,
		2,
		passDescritption.pIAInputSignature,
		passDescritption.IAInputSignatureSize,
		&inputLayout);
}