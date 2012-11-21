#include "KeyListener.h"
#include <Windows.h>

KeyListener::KeyListener(char pKey)
{
	isPushed = isDown = isReleased = false;
	key = pKey;
}

KeyListener::~KeyListener()
{

}

void KeyListener::update()
{
	if(GetAsyncKeyState(key)&0x8000)
	{
		if(!isPushed && !isDown && !isReleased)
		{
			isPushed = true;
			return;
		}
		else if(isPushed && !isDown && !isReleased)
		{
			isPushed = false;
			isDown = true;
		}
	}
	else if(isDown)
	{
		isDown = false;
		isReleased = true;
	}
	else
	{
		isReleased = false;
	}
}