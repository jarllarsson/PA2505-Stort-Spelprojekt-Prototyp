/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/12/2012 												*/
/************************************************************************/

#ifndef KEYLISTENER_H__
#define KEYLISTENER_H__

#include <Windows.h>
#include "stdafx.h"
#include "EventInterface.h"

class KeyListener
{
private:
	KeyListenType	mListenType;
	WPARAM			mKey;
	bool			mIsActive;
	EventInterface*	mCallbackFuntion;
public:
	KeyListener(WPARAM key,KeyListenType type, EventInterface* callbackFuntion);
	~KeyListener();
	bool Update(WPARAM input);
	KeyListenType GetListenType();
	bool GetIsActive();
};
#endif // KEYLISTENER_H__
