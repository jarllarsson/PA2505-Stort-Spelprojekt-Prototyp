/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/18/2012 												*/
/************************************************************************/

#ifndef LIGHTHANDLER_H__
#define LIGHTHANDLER_H__

#include "PointLight.h"
#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include <vector>

using namespace std;

class LightHandler
{
private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

	Shader*	mShader;
	Buffer* mPointLightBuffer;

	vector<PointLight>* mPointLights;
public:
	LightHandler();
	~LightHandler();
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader);
	HRESULT CreatePointLightBuffer();
	HRESULT AddPointLight(PointLight light);
	HRESULT RemovePointLight(unsigned int index);
	HRESULT Draw();
};
#endif // LIGHTHANDLER_H__
