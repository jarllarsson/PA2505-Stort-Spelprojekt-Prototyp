#ifndef MovementComp_h__
#define MovementComp_h__
#include "Component.h"
#include "Object.h"
#include <D3D10.h>

class MovementComp : public Component
{
private:
	Object* parent;
	static const int velocity = 20;
	static const int radius = 30;
	float x;
	float z;
	D3DXVECTOR3 originalPos;
protected:
public:
	MovementComp(Object* pParent);
	virtual ~MovementComp();
	HRESULT init();
	HRESULT update(float deltaTime);
};
#endif // MovementComp_h__
