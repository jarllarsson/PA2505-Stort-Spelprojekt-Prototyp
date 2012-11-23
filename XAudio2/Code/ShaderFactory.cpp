#include "ShaderFactory.h"

ShaderFactory::ShaderFactory(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

ShaderFactory::~ShaderFactory()
{
	for (unsigned int i = 0; i < m_shaderList.size(); i++)
	{
		delete m_shaderList.at(i).second;
	}
}

Shader* ShaderFactory::CreateBasicDeferredShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(BASIC_DEFERRED);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0	},
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		if(FAILED(newShader->Init(device,deviceContext, "../Shaders/BasicDeferred.fx", inputDesc, 3)))
		{
			newShader = NULL;
		}
		else
		{
			PushBackNewlyCreatedShader(BASIC_DEFERRED, newShader);
		}
	}

	return newShader;
}

Shader* ShaderFactory::CreateAdvanceDeferredShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(ADVANCE_DEFERRED);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
			{ "TEXCORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
			{ "NORMAL",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
		};
		if(FAILED(newShader->Init(device,deviceContext, "../Shaders/AdvanceDeferred.fx", inputDesc, 3)))
		{
			newShader = NULL;
		}
		else
		{
			PushBackNewlyCreatedShader(ADVANCE_DEFERRED, newShader);
		}
	}

	return newShader;
}

Shader* ShaderFactory::CreateDeferredShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(DEFERRED);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
			{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		if (FAILED(newShader->Init(device,deviceContext,"../Shaders/DeferredShader.fx", inputDesc, 2)))
		{
			newShader = NULL;
		}
		else
		{
			PushBackNewlyCreatedShader(DEFERRED, newShader);
		}
	}
	return newShader;
}


Shader* ShaderFactory::CreatePointLightShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(POINT_LIGHT);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		if (FAILED(newShader->Init(device,	deviceContext,	"../Shaders/PointLight.fx",	inputDesc,	1)))
		{
			newShader = NULL;
		}
		else
		{
			PushBackNewlyCreatedShader(POINT_LIGHT, newShader);
		}
	}
	return newShader;
}

Shader* ShaderFactory::CreateBoundBoxShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(AABB_SHADER);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		if ( FAILED(newShader->Init(device, deviceContext, "../Shaders/BoundingShader.fx", inputDesc, 1)))
		{
			newShader = NULL;
		}
		else
		{
			PushBackNewlyCreatedShader(AABB_SHADER, newShader);
		}
	}

	return newShader;
}
Shader* ShaderFactory::CreateSSAOShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(SSAO_SHADER);
	if( newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
			{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		if (FAILED(newShader->Init(device,deviceContext,"../Shaders/SSAO.fx", inputDesc, 2)))
			newShader = NULL;
		else
			PushBackNewlyCreatedShader(SSAO_SHADER, newShader);
	}

	return newShader;
}

Shader* ShaderFactory::CreateScreenAligned()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(SCREEN_ALIGNED);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0},
			{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		if (FAILED(newShader->Init(device,deviceContext,"../Shaders/ScreenAligned.fx", inputDesc, 2)))
			newShader = NULL;
		else
			PushBackNewlyCreatedShader(SCREEN_ALIGNED, newShader);
	}

	return newShader;
}

Shader* ShaderFactory::CreateShadowMapShader()
{
	Shader* newShader = CheckIfShaderHasBeenCreated(SHADOW_SHADER);
	if (newShader == NULL)
	{
		newShader = new Shader();
		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0	},
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		if (FAILED(newShader->Init(device,deviceContext,"../Shaders/SShadowMapShader.fx",inputDesc,3)))
			newShader = NULL;
		else
			PushBackNewlyCreatedShader(SHADOW_SHADER, newShader);
	}

	return newShader;
}


int ShaderFactory::FindShaderInList(TYPES_OF_SHADERS p_shaderType)
{
	for (unsigned int i = 0; i < m_shaderList.size(); i++)
	{
		if(m_shaderList.at(i).first == p_shaderType)
			return i;
	}

	return -1;
}

Shader* ShaderFactory::CheckIfShaderHasBeenCreated(TYPES_OF_SHADERS p_shaderType)
{
	int index = FindShaderInList(p_shaderType);

	Shader* newShader = NULL;

	if(index != -1)
		newShader = m_shaderList.at(index).second;

	return newShader;
}

void ShaderFactory::PushBackNewlyCreatedShader(TYPES_OF_SHADERS p_shaderType, Shader* p_newShader)
{
	pair<TYPES_OF_SHADERS,Shader*> newPair;
	newPair.first = p_shaderType;
	newPair.second = p_newShader;

	m_shaderList.push_back(newPair);
}