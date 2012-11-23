#include "MovementEventListener.h"

MovementEventListener::MovementEventListener()
{
	activeKeys = new vector<KeyListener*>();
	activeMouseListeners = new vector<EventInterface*>();
}

MovementEventListener::~MovementEventListener()
{
	for (unsigned int i = 0; i < activeKeys->size(); i++)
		delete activeKeys->at(i);
	
	delete activeKeys;

	for (unsigned int i = 0; i < activeMouseListeners->size(); i++)
		delete activeMouseListeners->at(i);

	delete activeMouseListeners;
}
void MovementEventListener::AddKeyListener(WPARAM key,KeyListenType type, EventInterface* keyInterface)
{
	KeyListener* newKeyListener = new KeyListener(key,type,keyInterface);
	activeKeys->push_back(newKeyListener);
}

void MovementEventListener::AddMouseListener(EventInterface* keyInterface)
{
	activeMouseListeners->push_back(keyInterface);
}

void MovementEventListener::NewKeyEvent(KeyListenType type, WPARAM wParam)
{
	for (unsigned int i = 0; i < activeKeys->size(); i++)
	{
		if (activeKeys->at(i)->GetListenType() == ANY || type == activeKeys->at(i)->GetListenType())
		{
			activeKeys->at(i)->Update(wParam);
		}
	}
}

void MovementEventListener::NewMouseMovement()
{
	for (unsigned int i = 0; i < activeMouseListeners->size(); i++)
	{
		activeMouseListeners->at(i)->PerformAction();
	}
}