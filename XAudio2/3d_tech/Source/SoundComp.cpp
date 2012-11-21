#include "SoundComp.h"
#include "InputHandler.h"

SoundComp::SoundComp(Object* pParent,SFX* pSource, char pKey)
{
	parent = pParent;
	source = pSource;
	key = pKey;
}
SoundComp::~SoundComp()
{

}
HRESULT SoundComp::init()
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT SoundComp::update(float deltaTime)
{
	HRESULT hr = S_OK;
	if (InputHandler::getInstance()->checkIfKeyIsPressed(key))
	{	
		if (!source->isPlaying())
		{
			source->play();
		}
		else
			source->stop();
	}

	source->setPos(parent->getPos());
	source->update();
	return hr;
}