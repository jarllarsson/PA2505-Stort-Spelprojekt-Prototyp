#include "MovementComp.h"

MovementComp::MovementComp(Object* pParent, int pRadius, float pVelocity)
{
	parent		= pParent;
	radius		= pRadius;
	velocity	= pVelocity;
	originalPos = parent->getPos();
	x = z = 0;
}
MovementComp::~MovementComp()
{

}

HRESULT MovementComp::init()
{
	HRESULT hr = S_OK;

	return hr;
}
HRESULT MovementComp::update(float deltaTime)
{
	HRESULT hr = S_OK;
	x += deltaTime*0.5f;
	z += deltaTime*0.5f;

	if(z>TWO_PI){
		z -= TWO_PI;
		x -= TWO_PI;
	}

	float xPos = cos(x)*radius;
	float zPos = sin(z)*radius;

	D3DXVECTOR3 pos =  parent->getPos();
	pos.z = zPos+originalPos.z;
	pos.x = xPos+originalPos.x;

	parent->setPos(pos);
	return hr;
}