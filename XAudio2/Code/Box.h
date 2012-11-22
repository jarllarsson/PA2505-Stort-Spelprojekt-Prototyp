/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/9/2012 												*/
/************************************************************************/

#ifndef BOX_H__
#define BOX_H__

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "Object.h"

class Box : public Object
{
private:
	struct AVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 texturePos;
		D3DXVECTOR3 normal;
	};
	float					rotationY;
	float					scale;
	int						dir;
	Shader*					shader;
	
	ID3D11ShaderResourceView* mAlbedoMap;
	ID3D11ShaderResourceView* mNormalMap;
	ID3D11ShaderResourceView* mSpecularMap;

	ID3D11Buffer*			vertexBuffer;
	ID3D11Buffer*			indexBuffer;

	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;
public:
	Box();
	Box(D3DXVECTOR3 pos);
	~Box();
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader);
	HRESULT Render(D3DXMATRIX viewProjection);
	HRESULT Update(float deltaTime);
};
#endif // BOX_H__