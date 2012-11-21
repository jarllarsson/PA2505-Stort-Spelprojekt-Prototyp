#include "InputHandler.h"
#include "Camera.h"
#include "Globals.h"
#include "SoundManager.h"
#include <Windows.h>

InputHandler* InputHandler::sInstance = 0;

InputHandler* InputHandler::getInstance()
{
	if (sInstance == 0)
	{
		sInstance = new InputHandler();
	}
	return sInstance;
}

void InputHandler::destroy()
{
	delete sInstance;
}

InputHandler::InputHandler()
{
	inverted = -1;
	mouseSensitivty = 1.0f;
	playPause = new KeyListener('P');
	forward = new KeyListener('W');
	backward = new KeyListener('S');
	strafeLeft = new KeyListener('A');
	strafeRight = new KeyListener('D');
	gunshot = new KeyListener('O');
	surroundMusic = new KeyListener('I');
	heliCopters = new KeyListener('U');

	keyListeners.push_back(surroundMusic);
	keyListeners.push_back(playPause);
	keyListeners.push_back(forward);
	keyListeners.push_back(backward);
	keyListeners.push_back(strafeLeft);
	keyListeners.push_back(strafeRight);
	keyListeners.push_back(gunshot);
	keyListeners.push_back(heliCopters);
}

InputHandler::~InputHandler()
{
	for (unsigned int i= 0; i < keyListeners.size();i++)
	{
		delete keyListeners[i];
	}
}

void InputHandler::update(float deltaTime)
{
	if(isActive){
		checkKeyboardInput(deltaTime);
		checkMousePos(deltaTime);
	}
}
void InputHandler::checkKeyboardInput(float deltaTime)
{
	for (unsigned int i = 0; i < keyListeners.size();i++)
	{
		keyListeners[i]->update();
	}
	if(playPause->getIsPushed())
		SoundManager::getInstace()->startStopMusic();
}
void InputHandler::checkMousePos(float deltaTime)
{
	POINT point;
	GetCursorPos(&point);

	SetCursorPos((int)HALF_SCREEN_WIDTH,(int)(HALF_SCREEN_HEIGHT));
	float turn = inverted*(HALF_SCREEN_WIDTH - point.x)*mouseSensitivty;
	float pitch = inverted*(HALF_SCREEN_HEIGHT - point.y)*mouseSensitivty;

	Camera::getInstance()->adjustHeadinPitch(turn*0.001f,pitch*0.001f);
}
bool InputHandler::checkIfKeyIsReleased(char pChar)
{
	for (unsigned int i = 0; i < keyListeners.size(); i++)
	{
		if(keyListeners[i]->getChar() == pChar)
			return keyListeners[i]->getIsReleased();
	}
	return false;
}

bool InputHandler::checkIfKeyIsPressed(char pChar)
{
	for( unsigned int i = 0; i < keyListeners.size(); i++)
	{
		if(keyListeners[i]->getChar() == pChar)
			return keyListeners[i]->getIsPushed();
	}
	return false;
}
bool InputHandler::checkIfKeyIsDown(char pChar)
{
	for( unsigned int i = 0; i < keyListeners.size(); i++)
	{
		if(keyListeners[i]->getChar() == pChar)
			return keyListeners[i]->getIsDown();
	}
	return false;
}