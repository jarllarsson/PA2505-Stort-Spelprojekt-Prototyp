#include "ObjectFactory.h"


ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
	SAFE_DELETE(mShaderFactory);
	SAFE_DELETE(mObjParser);

	for (unsigned int i = 0; i< m_meshList.size(); i++)
	{
		delete m_meshList[i].second->data->indexBuffer;
		delete m_meshList[i].second->data->vertexBuffer;
		delete m_meshList[i].second;
	}
	m_meshList.clear();
}

HRESULT ObjectFactory::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* gameCamera)
{
	HRESULT hr = S_OK;

	mDevice			= device;
	mDeviceContext	= deviceContext;
	mGameCamera		= gameCamera;
	mObjParser		= new LoadObj();

	mShaderFactory = new ShaderFactory(mDevice,mDeviceContext);

	return hr;
}

Billboard* ObjectFactory::CreateABillboard(D3DXVECTOR3 pos,float radius)
{
	Billboard* newBillboard = new Billboard();

	newBillboard->Init(mDevice,mDeviceContext,mGameCamera,pos,radius);

	return newBillboard;
}

PointLight* ObjectFactory::CreateAPointLight(Shader* shader,D3DXVECTOR3 pos,D3DXCOLOR diffuse, float range)
{
	PointLight* newPointLight = new PointLight();
	ID3D11BlendState* blendState;
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory( &blendStateDesc, sizeof( D3D11_BLEND_DESC ) );
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if( FAILED( mDevice->CreateBlendState( &blendStateDesc, &blendState ) ) )
	{
		return NULL;
	}

	ID3D11RasterizerState* newRasterState;
	D3D11_RASTERIZER_DESC rasterState;
	rasterState.FillMode = D3D11_FILL_SOLID;
	rasterState.CullMode = D3D11_CULL_NONE;
	rasterState.AntialiasedLineEnable = false;
	rasterState.FrontCounterClockwise = false;
	rasterState.DepthBias = 0;
	rasterState.DepthBiasClamp = 0;
	rasterState.SlopeScaledDepthBias = 0;
	rasterState.DepthClipEnable = false;
	rasterState.ScissorEnable = false;
	rasterState.MultisampleEnable = false;

	HRESULT hr = mDevice->CreateRasterizerState(&rasterState,&newRasterState);

	AABB* newAABB = CreateAAABB(D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1),diffuse);
	newPointLight->Init(blendState,newRasterState,mDeviceContext,CreateABox(),newAABB,shader,pos,diffuse,range);

	return newPointLight;
}

Mesh* ObjectFactory::CreateAMeshFromObj(D3DXVECTOR3 pos, string filename,D3DXCOLOR p_debugColor)
{
	HRESULT hr;
	ID3D11ShaderResourceView* albedoTexture;
	hr = D3DX11CreateShaderResourceViewFromFile(mDevice,"../Textures/bthcolor.dds",0,0,&albedoTexture,0);

	ObjData* objData = CheckIfMeshHasBeenCreated(filename);
	if(objData == NULL)
	{
		MeshData* data = mObjParser->readObjFile(MESH_PATH+filename);

		BufferInfo vertex,index;
		vertex.size = sizeof(Vertex);
		vertex.numElements = data->vertices.size();
		vertex.data = &data->vertices[0];
		vertex.bufferType = VERTEX_BUFFER;

		index.size = sizeof(DWORD);
		index.numElements = data->indices.size();
		index.data = &data->indices[0];
		index.bufferType = INDEX_BUFFER;

		MeshBuffers* meshBuffers = new MeshBuffers();
		meshBuffers->vertexBuffer = CreateMeshBuffers(vertex);
		meshBuffers->indexBuffer = CreateMeshBuffers(index);

		MinMax minMax = FindMinMax(data->position);

		objData = new ObjData();
		objData->data = meshBuffers;
		objData->minMax	= minMax;

		PushBackNewlyCreatedMesh(filename,objData);
	}

	AABB* newAABB = CreateABoundboxForMesh(objData->minMax,p_debugColor);
	Mesh* newMesh = new Mesh(mDevice,mDeviceContext,mShaderFactory->CreateBasicDeferredShader(),albedoTexture,objData,pos, newAABB);

	return newMesh;
}
AABB* ObjectFactory::CreateAAABB(D3DXVECTOR3 p_center, D3DXVECTOR3 p_halfDimension, D3DXCOLOR p_color)
{
	AABB* newAABB;

	D3DXVECTOR3 min = D3DXVECTOR3(p_center.x-p_halfDimension.x, p_center.y-p_halfDimension.y, p_center.z-p_halfDimension.z);
	D3DXVECTOR3 max = D3DXVECTOR3(p_center.x+p_halfDimension.x, p_center.y+p_halfDimension.y, p_center.z+p_halfDimension.z);

	MinMax data(min,max);

	newAABB = CreateABoundboxForMesh(data,p_color);

	return newAABB;
}
AABB* ObjectFactory::CreateABoundboxForMesh(MinMax p_minMax, D3DXCOLOR p_color)
{
	AABB* newAABB;

	D3DXVECTOR3* mesh =	p_minMax.getVCorners();
	DWORD* indicies = p_minMax.getICorners();

	BufferInfo vertex,index;
	vertex.size = sizeof(D3DXVECTOR3);
	vertex.numElements = 8;
	vertex.data = &mesh[0];
	vertex.bufferType = VERTEX_BUFFER;

	index.size = sizeof(DWORD);
	index.numElements = 24;
	index.data = &indicies[0];
	index.bufferType = INDEX_BUFFER;

	MeshBuffers meshBuffers;
	meshBuffers.vertexBuffer = CreateMeshBuffers(vertex);
	meshBuffers.indexBuffer = CreateMeshBuffers(index);
	
	delete mesh;
	delete indicies;
	newAABB = new AABB(	mDeviceContext,p_color,p_minMax,meshBuffers.vertexBuffer,
						meshBuffers.indexBuffer,mShaderFactory->CreateBoundBoxShader());

	return newAABB;
}

ScreenAligned* ObjectFactory::CreateAScreenAlignedQuad(D3DXVECTOR2 p_min, D3DXVECTOR2 p_max,
	ID3D11ShaderResourceView* p_samplerTarget, D3DXVECTOR4 p_activeSampleChannels, int negateSample/* =0 */)
{
	struct PTVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

	PTVertex mesh[]= {
		{D3DXVECTOR3( p_min.x, p_min.y, 0),	D3DXVECTOR2(1,1)},
		{D3DXVECTOR3( p_max.x, p_min.y, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( p_min.x, p_max.y,	0),	D3DXVECTOR2(1,0)},

		{D3DXVECTOR3( p_max.x, p_min.y, 0),	D3DXVECTOR2(0,1)},
		{D3DXVECTOR3( p_min.x, p_max.y, 0),	D3DXVECTOR2(1,0)},
		{D3DXVECTOR3( p_max.x, p_max.y, 0),	D3DXVECTOR2(0,0)},
	};

	BufferInfo vertex;
	vertex.bufferType = VERTEX_BUFFER;
	vertex.data = &mesh[0];
	vertex.numElements = 6;
	vertex.size = sizeof(PTVertex);

	Buffer* newVertexBuffer = CreateMeshBuffers(vertex);

	return new ScreenAligned	(mDeviceContext,p_samplerTarget,mShaderFactory->CreateScreenAligned(),
								newVertexBuffer,p_activeSampleChannels, false, negateSample);
}

MinMax ObjectFactory::FindMinMax(vector<D3DXVECTOR3> p_positions)
{
	MinMax data;

	float x,y,z;

	for(unsigned int i = 0 ; i < p_positions.size(); i++)
	{
		x = p_positions.at(i).x;
		y = p_positions.at(i).y;
		z = p_positions.at(i).z;

		if (x < data.min.x)
			data.min.x = x;
		else if (x > data.max.x)
			data.max.x = x;

		if (y < data.min.y)
			data.min.y = y;
		else if (y > data.max.y)
			data.max.y = y;

		if (z < data.min.z)
			data.min.z = z;
		else if (z > data.max.z)
			data.max.z = z;
	}
	return data;
}


MeshBuffers* ObjectFactory::CreateABox()
{
	struct AVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 texturePos;
		D3DXVECTOR3 normal;
	};

	AVertex mesh[] = {
		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(1,1,-1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,0,-1)},

		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(1,-1,1),		D3DXVECTOR2(0,1),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(-1,1,1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(0,0,1)},

		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(-1,1,1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(1,1,-1),		D3DXVECTOR2(1,1),	D3DXVECTOR3(0,1,0)},

		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(1,-1,1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(0,-1,0)},

		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,1,1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(-1,0,0)},

		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,1,-1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,-1,1),		D3DXVECTOR2(1,1),	D3DXVECTOR3(1,0,0)}
	};

	DWORD indices[] = {
		0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23
	};

	BufferInfo vertexBufferDesc;
	vertexBufferDesc.size = sizeof(AVertex);
	vertexBufferDesc.data = &mesh[0];
	vertexBufferDesc.numElements = 24;
	vertexBufferDesc.bufferType = VERTEX_BUFFER;

	Buffer* vertexBuffer = CreateMeshBuffers(vertexBufferDesc);

	BufferInfo indexBufferDesc;
	indexBufferDesc.size = sizeof(DWORD);
	indexBufferDesc.data = &indices[0];
	indexBufferDesc.numElements = 36;
	indexBufferDesc.bufferType = INDEX_BUFFER;

	Buffer* indexBuffer = CreateMeshBuffers(indexBufferDesc);

	MeshBuffers* newMeshBuffer = new MeshBuffers();
	newMeshBuffer->indexBuffer = indexBuffer;
	newMeshBuffer->vertexBuffer = vertexBuffer;

	return newMeshBuffer;
}

Frustum* ObjectFactory::CreateAFrustum(CameraSettings p_settings, Shader* p_shader, D3DXCOLOR p_color)
{
	Frustum* newFrustum = NULL;
	D3DXMATRIX view, proj;
	Camera* newCamera = new Camera(NULL,p_settings);

	view = newCamera->getViewMatrix();
	proj = newCamera->getProjectionMatrix();

	struct Vertex
	{
		D3DXVECTOR4 pos;
	};

	Vertex mesh[] = {
		//near
		D3DXVECTOR4(-1, -1, -1, 1),
		D3DXVECTOR4(1, -1, -1, 1),
		D3DXVECTOR4(1,  1, -1, 1),
		D3DXVECTOR4(-1,  1, -1, 1),
		//far
		D3DXVECTOR4(-1, -1, 1, 1),
		D3DXVECTOR4(1, -1, 1, 1),
		D3DXVECTOR4(1,  1, 1, 1),
		D3DXVECTOR4(-1,  1, 1, 1)
	};

	D3DXMATRIX viewProj, vpInv;
	viewProj = view * proj;
	D3DXMatrixInverse(&vpInv, 0, &viewProj);

	//transform to world coords
	for(int i = 0; i < 8; i++)
	{
		D3DXVec4Transform(&mesh[i].pos, &mesh[i].pos, &vpInv);

		mesh[i].pos.x /= mesh[i].pos.w;
		mesh[i].pos.y /= mesh[i].pos.w;
		mesh[i].pos.z /= mesh[i].pos.w;
		mesh[i].pos.w = 1.0f;
	}

	vector<Vertex> linePoints;

	//define line list vertices
	int lineIndices[] = {
		0,1,1,2,2,3,3,0, 
		4,5,5,6,6,7,7,4,
		0,4,1,5,2,6,3,7
	};
	for(int i = 0; i < sizeof(lineIndices) / sizeof(int); i++)
		linePoints.push_back(mesh[lineIndices[i]]);

	vector<D3DXVECTOR3> points;
	for(unsigned int i = 0; i < 8; i++)
		points.push_back(D3DXVECTOR3(mesh[i].pos));
	
	BufferInfo vertex;
	vertex.size = sizeof(Vertex);
	vertex.numElements = linePoints.size();
	vertex.data = &linePoints[0];
	vertex.bufferType = VERTEX_BUFFER;

	Buffer* vertexBuffer = CreateMeshBuffers(vertex);	
	
	newFrustum = new Frustum(this,mDeviceContext,points,vertexBuffer, p_shader,newCamera, p_color);

	return newFrustum;
}

Buffer* ObjectFactory::CreateMeshBuffers(BufferInfo p_buffer)
{	
	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = p_buffer.size;
	bufferDesc.InitData = p_buffer.data;
	bufferDesc.NumElements = p_buffer.numElements;
	bufferDesc.Type = p_buffer.bufferType;
	bufferDesc.Usage = BUFFER_DEFAULT;

	Buffer* newBuffer = new Buffer();
	if(FAILED(newBuffer->Init(mDevice,mDeviceContext,bufferDesc)))
		return NULL;
	
	return newBuffer;
}

int ObjectFactory::FindMeshInList( string p_mesh )
{
	for (unsigned int i = 0; i < m_meshList.size(); i++)
		if (m_meshList[i].first == p_mesh)
			return i;
	return -1;
}

ObjData* ObjectFactory::CheckIfMeshHasBeenCreated( string p_mesh )
{
	int index = FindMeshInList(p_mesh);
	
	ObjData* newObjData = NULL;

	if (index != -1)
		newObjData = m_meshList.at(index).second;

	return newObjData;
}

void ObjectFactory::PushBackNewlyCreatedMesh( string p_mesh, ObjData* p_objData)
{
	pair<string,ObjData*> newPair;
	newPair.first = p_mesh;
	newPair.second = p_objData;

	m_meshList.push_back(newPair);
}