#ifndef MovementComp_h__
#define MovementComp_h__
#include "Component.h"
#include "Object.h"
#include <D3D11.h>
#include "stdafx.h"

class MovementComp : public Component
{
private:
	Object*		parent;
	float		velocity;
	int			radius;
	float		x;
	float		z;
	D3DXVECTOR3 originalPos;
protected:
public:
	MovementComp(Object* pParent, int radius, float velocity);
	virtual ~MovementComp();
	HRESULT init();
	HRESULT update(float deltaTime);
};
#endif // MovementComp_h__
