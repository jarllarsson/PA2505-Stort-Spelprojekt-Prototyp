/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/9/2012 												*/
/************************************************************************/

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "stdafx.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <map>

enum TYPES_OF_SHADERS
{
	BASIC_DEFERRED, ADVANCE_DEFERRED, DEFERRED, LIGHT_SHADER, POINT_LIGHT, 
	AABB_SHADER, SSAO_SHADER, SCREEN_ALIGNED, SHADOW_SHADER

};

using namespace std;

class ShaderFactory
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	vector<pair<TYPES_OF_SHADERS,Shader*>>		m_shaderList;
private:
	int		FindShaderInList(TYPES_OF_SHADERS);
	Shader*	CheckIfShaderHasBeenCreated(TYPES_OF_SHADERS);
	void	PushBackNewlyCreatedShader(TYPES_OF_SHADERS,Shader*);
public:
	ShaderFactory(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ShaderFactory();
	Shader* CreateBasicDeferredShader();
	Shader*	CreateAdvanceDeferredShader();
	Shader* CreateDeferredShader();
	Shader* CreatePointLightShader();
	Shader* CreateBoundBoxShader();
	Shader*	CreateSSAOShader();
	Shader* CreateScreenAligned();
	Shader*	CreateShadowMapShader();
};
#endif // SHADERFACTORY_H__
