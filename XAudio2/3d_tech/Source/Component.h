#ifndef Component_h__
#define Component_h__
#include <d3d10.h>

class Component
{
private:
protected:
public:
	virtual HRESULT init() = 0;
	virtual HRESULT update(float deltaTime) = 0;
};
#endif // Component_h__
