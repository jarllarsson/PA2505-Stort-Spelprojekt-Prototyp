/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/23/2012 												*/
/************************************************************************/

#ifndef BILLBOARD_H__
#define BILLBOARD_H__

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"

class Billboard
{
private:
	float			size;

	D3DXVECTOR3*	data;

	D3DXVECTOR3		pos,
					right, 
					up,
					viewVector;
	Camera* camera;
	
	Buffer* buffer;
	
	D3DXMATRIX worldMatrix;

public:
	Billboard();
	~Billboard();
	HRESULT			Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* camera,D3DXVECTOR3 pos,float size);
	void			GenerateBillboard(D3DXVECTOR3 pos);
	D3DXMATRIX		GenerateWVPMatrix();
	D3DXMATRIX		GetWorldMatrix();
	void			ApplyBuffer();

};
#endif // BILLBOARD_H__
