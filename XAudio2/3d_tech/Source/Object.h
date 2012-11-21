#ifndef Object_h__
#define Object_h__
#include <d3dx10.h>
#include <d3d10.h>
#include "Camera.h"

class Object
{
protected:
	D3DXVECTOR3 pos;
protected:
public:
	Object(D3DXVECTOR3 pPos) {
		pos = pPos;
	}
	~Object(){
	}
	D3DXMATRIX& getFinalMatrix(){
		D3DXMATRIX world = D3DXMATRIX(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			pos.x,pos.y,pos.z,1
		);

		return world*Camera::getInstance()->getViewProjMatrix();
	}
	D3DXVECTOR3 getPos(){
		return pos;
	}
	void setPos(D3DXVECTOR3 pNewPos){
		pos = pNewPos;
	}
	virtual void draw() = 0;
	virtual void update(float deltaTime) = 0;
};
#endif // Object_h__
