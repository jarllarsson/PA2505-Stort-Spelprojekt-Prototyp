#ifndef Triangle_h__
#define Triangle_h__

#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"

class Triangle : public Object
{
private:
	Shader* shader;
	Buffer* buffer;
	float	rotationY;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
public:
	Triangle();
	~Triangle();
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader,D3DXVECTOR3 pos);
	HRESULT Render(D3DXMATRIX viewProjection);
	HRESULT Update(float deltaTime);
};
#endif // Triangle_h__
