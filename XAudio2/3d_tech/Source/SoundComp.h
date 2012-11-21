#ifndef SoundComp_h__
#define SoundComp_h__
#include "SFX.h"
#include "Object.h"
#include "Component.h"

class SoundComp : public Component
{
private:
	SFX* source;
	Object* parent;
	char key;
protected:
public:
	SoundComp(Object* pParent,SFX* pSource,char pKey);
	~SoundComp();
	HRESULT init();
	HRESULT update(float deltaTime);
};
#endif // SoundComp_h__
