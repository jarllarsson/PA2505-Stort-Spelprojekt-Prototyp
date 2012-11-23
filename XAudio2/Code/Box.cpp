#include "Box.h"

Box::Box() : Object(D3DXVECTOR3(0,0,0))
{
	shader			= NULL;
	vertexBuffer	= NULL;
	rotationY		= 0;
	scale			= 0;
}

Box::Box(D3DXVECTOR3 pos) : Object(pos)
{
	shader			= NULL;
	vertexBuffer	= NULL;
	rotationY		= 0;
	scale			= 0;
	dir				= 1;
}

Box::~Box()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

HRESULT Box::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader)
{
	HRESULT hr = S_OK;

	this->device = device;
	this->deviceContext = deviceContext;
	this->shader = shader;

	hr = D3DX11CreateShaderResourceViewFromFile(device,"../Textures/ccube_diffuse.png",0,0,&mAlbedoMap,0);
	hr = D3DX11CreateShaderResourceViewFromFile(device,"../Textures/ccube_normals.png",0,0,&mNormalMap,0);
	hr = D3DX11CreateShaderResourceViewFromFile(device,"../Textures/ccube_spec.png",0,0,&mSpecularMap,0);

	AVertex mesh[] = {
		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(1,1,-1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(0,0,-1)},
		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,0,-1)},
		
		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(1,-1,1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(0,0),	D3DXVECTOR3(0,0,1)},
		{  D3DXVECTOR3(-1,1,1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(0,0,1)},
		
		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(-1,1,1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(0,1,0)},
		{  D3DXVECTOR3(1,1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,1,0)},
		
		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(1,-1,1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(0,-1,0)},
		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(0,-1,0)},
		
		{  D3DXVECTOR3(-1,-1,1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,1,1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,1,-1),	D3DXVECTOR2(1,0),	D3DXVECTOR3(-1,0,0)},
		{  D3DXVECTOR3(-1,-1,-1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(-1,0,0)},
		
		{  D3DXVECTOR3(1,-1,-1),	D3DXVECTOR2(0,1),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,1,-1),	D3DXVECTOR2(0,0),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,1,1),		D3DXVECTOR2(1,0),	D3DXVECTOR3(1,0,0)},
		{  D3DXVECTOR3(1,-1,1),	D3DXVECTOR2(1,1),	D3DXVECTOR3(1,0,0)}
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(AVertex)*24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexInData;
	vertexInData.pSysMem = &mesh[0];

	hr = device->CreateBuffer(&vertexBufferDesc,&vertexInData,&vertexBuffer);

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

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(DWORD)*36;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexInData;
	indexInData.pSysMem = &indices[0];
	hr = device->CreateBuffer(&indexBufferDesc,&indexInData,&indexBuffer);

	return hr;
}

HRESULT Box::Update(float deltaTime)
{
	HRESULT hr = S_OK;
	rotY += deltaTime;
	rotZ += deltaTime;

	scaleLevel += deltaTime*0.5f*dir;

	if (scaleLevel>1)
		dir = -1;
	else if(scaleLevel<0)
		dir = 1;

	setScale(scaleLevel);

	return hr;
}

HRESULT Box::Render(D3DXMATRIX viewProjection)
{
	HRESULT hr = S_OK;
	D3DXMATRIX world, worldViewProjection;

	world = getFinalMatrix();
	worldViewProjection = world * viewProjection;

	shader->SetMatrix("gWVP", worldViewProjection);
	shader->SetMatrix("gWorld", world);
	shader->SetResource("gAlbedoMap",mAlbedoMap);
	shader->SetResource("gNormalMap",mNormalMap);
	shader->SetResource("gSpecularMap", mSpecularMap);
	shader->Apply(0);

	UINT stride = sizeof(AVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0,1,&vertexBuffer,&stride,&offset);
	deviceContext->IASetIndexBuffer(indexBuffer,DXGI_FORMAT_R32_UINT,0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(36,0,0);
	return hr;
}