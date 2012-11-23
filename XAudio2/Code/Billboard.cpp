#include "Billboard.h"

Billboard::Billboard()
{
	data = NULL;
}

Billboard::~Billboard()
{
	//SAFE_DELETE(shader);
	SAFE_DELETE(buffer);
	SAFE_DELETE(data);
}

HRESULT Billboard::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* camera,D3DXVECTOR3 pos,float size)
{
	HRESULT hr = S_OK;

	this->camera = camera;
	this->pos = pos;
	this->size = size;

	GenerateBillboard(pos);

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(D3DXVECTOR3);
	bufferDesc.InitData = data;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	buffer = new Buffer();
	buffer->Init(device,deviceContext,bufferDesc);

	return hr;
}

/*
 *	Must be called before GenerateWVPMatrix
 */
void Billboard::GenerateBillboard(D3DXVECTOR3 pos)
{
	float halfSize = size/2.0f;

	D3DXVECTOR3 viewVector = camera->getPos()-pos;
	D3DXVec3Normalize(&viewVector,&viewVector);
	D3DXVECTOR3 right,up;
	up = camera->getUp();
	D3DXVec3Cross(&right,&viewVector,&camera->getUp());

	right = camera->getRight();
	viewVector = camera->getForward();

	if (data != NULL)
		delete data;

	data = new D3DXVECTOR3[6];

	/*	Billboarden byggs upp av följande punkter 0,1,2,3,4,5
	 *  ______________
	 *	|1		2 / 3|
	 *	|			 |
	 *	|0 / 5		4|
	 *	--------------
	 */
	
	data[0] = D3DXVECTOR3( -halfSize,	-halfSize,	0);
	data[1] = D3DXVECTOR3( -halfSize,	halfSize,	0);
	data[2] = D3DXVECTOR3( halfSize,	halfSize,	0);
	data[3] = data[2];
	data[4] = D3DXVECTOR3( halfSize,	-halfSize,	0);
	data[5] = data[0];

	worldMatrix = D3DXMATRIX(
					right.x,		right.y,		right.z,		0.0f,
					up.x,			up.y,			up.z,			0.0f,
					viewVector.x,	viewVector.y,	viewVector.z,	0.0f,
					pos.x,			pos.y,			pos.z,			1.0f
					);
}

D3DXMATRIX Billboard::GenerateWVPMatrix()
{
	D3DXMATRIX wvp;

	wvp = worldMatrix*camera->getViewProjMatrix();

	return wvp;
}

D3DXMATRIX Billboard::GetWorldMatrix()
{
	return worldMatrix;
}

void Billboard::ApplyBuffer()
{
	buffer->Apply(0);
}