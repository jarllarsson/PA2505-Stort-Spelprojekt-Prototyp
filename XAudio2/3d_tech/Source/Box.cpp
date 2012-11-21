#include "Box.h"
#include "Camera.h"

Box::Box(ID3D10Device* pDevice,D3DXVECTOR3 pPos,D3DXCOLOR pColor, float pScale) : Object(pPos)
{
	device = pDevice;
	numVertices = 8;
	numIndices = 36;

	SVertex vertices[] = 
	{
		{D3DXVECTOR3(-1.0f,-1.0f,-1.0f),pColor},
		{D3DXVECTOR3(-1.0f,1.0f,-1.0f),pColor},
		{D3DXVECTOR3(1.0f,1.0f,-1.0f),pColor},
		{D3DXVECTOR3(1.0f,-1.0f,-1.0f),pColor},
		{D3DXVECTOR3(-1.0f,-1.0f,1.0f),pColor},
		{D3DXVECTOR3(-1.0f,1.0f,1.0f),pColor},
		{D3DXVECTOR3(1.0f,1.0f,1.0f),pColor},
		{D3DXVECTOR3(1.0f,-1.0f,1.0f),pColor}
	};

	for (DWORD i = 0; i < numVertices;i++)
	{
		vertices[i].Position *= pScale;
	}

	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SVertex)*numVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HRESULT hr = S_OK;
	hr = device->CreateBuffer(&vbd,&vinitData,&vertexBuffer);

	DWORD indices[] = {
		0,1,2,
		0,2,3,

		4,6,5,
		4,7,6,

		4,5,1,
		4,1,0,

		3,2,6,
		3,6,7,

		1,5,6,
		1,6,2,
		
		4,0,3,
		4,3,7
	};

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD)*numIndices;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = device->CreateBuffer(&vbd,&iinitData,&indexBuffer);

	initPipeline();
}

Box::~Box()
{
	if (movComp != NULL)
		delete movComp;

	delete soundComp;
}

void Box::update(float deltaTime)
{
	if (movComp != NULL)
	{
		movComp->update(deltaTime);
	}
	soundComp->update(deltaTime);

}
void Box::resolve(SFX* sound,char key,bool enableMovement)
{
	if (enableMovement)
		movComp = new MovementComp(this);
	else
		movComp = NULL;

	soundComp = new SoundComp(this,sound,key);
}
void Box::draw()
{
	worldMatrix->SetMatrix((float*)&getFinalMatrix());
	effectPass->Apply(0);

	UINT stride = sizeof(SVertex);
	UINT offset = 0;

	device->IASetInputLayout(inputLayout);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	device->IASetVertexBuffers(0,1,&vertexBuffer,&stride,&offset);
	device->IASetIndexBuffer(indexBuffer,DXGI_FORMAT_R32_UINT,0);
	device->DrawIndexed(36,0,0);
}

void Box::initPipeline()
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