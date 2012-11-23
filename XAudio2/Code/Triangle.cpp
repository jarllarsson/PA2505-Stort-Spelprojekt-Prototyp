#include "Triangle.h"


Triangle::Triangle() : Object(D3DXVECTOR3(0,0,0))
{
	shader = NULL;
	buffer = NULL;
	rotationY = 0.0f;
}

Triangle::~Triangle()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(buffer);
}

HRESULT Triangle::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader,D3DXVECTOR3 pos)
{
	HRESULT hr = S_OK;
	this->shader = shader;
	this->device = device;
	this->deviceContext = deviceContext;
	this->pos = pos;
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR color;
	};

	Vertex mesh[] = {
		{pos+D3DXVECTOR3(1,0,0), RED},
		{pos+D3DXVECTOR3(-1,0,0), GREEN},
		{pos+D3DXVECTOR3(0,1,0), BLUE}
	};

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(Vertex);
	bufferDesc.InitData = mesh;
	bufferDesc.NumElements = 3;
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	buffer = new Buffer();
	hr = buffer->Init(device,deviceContext,bufferDesc);

	return hr;
}
HRESULT Triangle::Update(float deltaTime)
{
	rotationY += deltaTime;

	return S_OK;
}
HRESULT Triangle::Render(D3DXMATRIX viewProjection)
{
	HRESULT hr = S_OK;
	//calculate WVP matrix
	D3DXMATRIX world, wvp;
	D3DXMatrixRotationY(&world, rotationY);
	wvp = world * viewProjection;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//draw triangle
	buffer->Apply();
	shader->SetMatrix("gWVP", wvp);
	shader->Apply(0);
	deviceContext->Draw(3,0);

	return hr;
}