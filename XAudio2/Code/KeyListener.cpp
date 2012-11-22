#include "KeyListener.h"

KeyListener::KeyListener(WPARAM key,KeyListenType type, EventInterface* callbackFuntion)
{
	mListenType			= type;
	mKey				= key;
	mIsActive			= false;
	mCallbackFuntion	= callbackFuntion;
}

KeyListener::~KeyListener()
{

}

bool KeyListener::Update(WPARAM input)
{
	if (input == mKey)
	{
		if (GetAsyncKeyState(input)&0x8000)
		{
			mIsActive = true;
			mCallbackFuntion->PerformAction();
		}
		else
			mIsActive = false;
	}
	return mIsActive;
}

KeyListenType KeyListener::GetListenType()
{
	return mListenType;
}

bool KeyListener::GetIsActive()
{
	return mIsActive;
}