/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/12/2012 												*/
/************************************************************************/

#ifndef MOVEMENTEVENTLISTENER_H__
#define MOVEMENTEVENTLISTENER_H__

#include <vector>
#include "EventInterface.h"
#include "KeyListener.h"


using namespace std;

class MovementEventListener
{
private:
	vector<KeyListener*>* activeKeys;
	vector<EventInterface*>* activeMouseListeners;
public:
	MovementEventListener();
	~MovementEventListener();
	void AddKeyListener(WPARAM key,KeyListenType type,EventInterface* keyInterface);
	void NewKeyEvent(KeyListenType type, WPARAM wParam);
	void AddMouseListener(EventInterface* keyInterface);
	void NewMouseMovement();

};
#endif // MOVEMENTEVENTLISTENER_H__
