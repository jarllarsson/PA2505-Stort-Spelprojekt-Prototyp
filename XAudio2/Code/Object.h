#ifndef Object_H
#define Object_H

#include "stdafx.h"

class Object
{
protected:
	D3DXVECTOR3 pos;
	
	D3DXMATRIX scaleMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX translationMatrix;

	float rotX,rotY,rotZ;
	float scaleLevel;
public:
	Object(D3DXVECTOR3 pPos) {
		D3DXMatrixIdentity(&scaleMatrix);
		D3DXMatrixIdentity(&rotationMatrix);

		pos = pPos;
		D3DXMatrixTranslation(&translationMatrix,pos.x,pos.y,pos.z);

		rotX = rotY = rotZ = 0;
		scaleLevel = 1;
	}
	~Object(){
	}
	D3DXMATRIX getFinalMatrix(){
		
		D3DXMatrixScaling(&scaleMatrix,scaleLevel,scaleLevel,scaleLevel);

		D3DXMATRIX transformationMatrix,rotXMa,rotYMa,rotZMa;
		
		D3DXMatrixTranslation(&translationMatrix,pos.x,pos.y,pos.z);

		D3DXMatrixRotationX(&rotXMa,rotX);
		D3DXMatrixRotationY(&rotYMa,rotY);
		D3DXMatrixRotationZ(&rotZMa,rotZ);

		rotationMatrix = rotXMa*rotYMa*rotZMa;

		transformationMatrix = scaleMatrix*rotationMatrix*translationMatrix;

		return transformationMatrix;
	}

	D3DXVECTOR3 getPos(){
		return pos;
	}

	float getScaleLevel()
	{
		return scaleLevel;
	}
	void setScale(float p_scaleLevel)
	{
		scaleLevel = p_scaleLevel;
	}

	void setRotation(D3DXMATRIX pRotation)
	{
		rotationMatrix = pRotation;
	}

	void setTranslation(D3DXMATRIX pTranslation)
	{
		translationMatrix = pTranslation;
	}

	void setPos(D3DXVECTOR3 pNewPos){
		pos = pNewPos;
	}
	virtual HRESULT Update(float deltaTime) = 0;
	virtual HRESULT Render(D3DXMATRIX viewProjection) = 0;
};
#endif // Object_h__