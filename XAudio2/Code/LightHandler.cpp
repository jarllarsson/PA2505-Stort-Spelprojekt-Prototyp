#include "LightHandler.h"

LightHandler::LightHandler()
{

}

LightHandler::~LightHandler()
{

}

HRESULT LightHandler::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader)
{
	HRESULT hr = S_OK;

	mDevice = device;
	mDeviceContext = deviceContext;
	mShader = shader;

	mPointLights = new vector<PointLight>(100);

	return hr;
}

HRESULT LightHandler::AddPointLight(PointLight light)
{
	HRESULT hr = S_OK;
	mPointLights->push_back(light);

	CreatePointLightBuffer();
	return hr;
}
HRESULT LightHandler::RemovePointLight(unsigned int index)
{
	HRESULT hr = S_OK;
	
	/*
	 *	TODO! Implement remove at index
	 */

	return hr;
}

HRESULT LightHandler::CreatePointLightBuffer()
{
	HRESULT hr = S_OK;

	if(mPointLightBuffer)
		SAFE_DELETE(mPointLightBuffer);

	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(PointLight);
	bufferDesc.InitData = mPointLights;
	bufferDesc.NumElements = mPointLights->size();
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	mPointLightBuffer = new Buffer();
	mPointLightBuffer->Init(mDevice,mDeviceContext,bufferDesc);

	return hr;
}
HRESULT LightHandler::Draw()
{
	HRESULT hr = S_OK;

	mDeviceContext->IASetInputLayout(NULL);
	mPointLightBuffer->Apply(0);
	mShader->Apply(0);
	mDeviceContext->Draw(mPointLights->size(),0);

	return hr;
}