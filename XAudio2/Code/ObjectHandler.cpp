#include "ObjectHandler.h"


ObjectHandler::ObjectHandler()
{
	objects = NULL;
}

ObjectHandler::~ObjectHandler()
{

}

HRESULT ObjectHandler::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

	shaderFactory = new ShaderFactory(device,deviceContext);

	HRESULT hr = S_OK;
	
	Triangle* triangle = new Triangle();
	if(FAILED(triangle->Init(device,deviceContext,shaderFactory->CreateBasicDeferredShader(),D3DXVECTOR3(0,0,0) ) ) )
		return E_FAIL;

	return hr;
}

HRESULT ObjectHandler::Update(float deltaTime)
{
	HRESULT hr = S_OK;

	for (unsigned int i = 0 ; i < objects->size(); i++)
	{
		objects->at(i)->Update(deltaTime);
	}

	return hr;
}

Object* ObjectHandler::GetObject(unsigned int index)
{
	return objects->at(index);
}

int ObjectHandler::GetNumObjects()
{
	return objects->size();
}