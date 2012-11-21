#ifndef Movement_h__
#define Movement_h__
#include <D3DX10.h>
#include <vector>
#include "KeyListener.h"

class InputHandler
{
private:
	static InputHandler* sInstance;

	bool isActive;
	short inverted;
	float mouseSensitivty;
	std::vector<KeyListener*> keyListeners;
	KeyListener* playPause;
	KeyListener* gunshot;
	KeyListener* surroundMusic;
	KeyListener* forward;
	KeyListener* backward;
	KeyListener* strafeLeft;
	KeyListener* strafeRight;
	KeyListener* heliCopters;
private:
	void checkKeyboardInput(float deltaTime);
	void checkMousePos(float deltaTime);
	InputHandler();
	~InputHandler();
protected:
public:
	static InputHandler* getInstance();
	static void destroy();
	void update(float deltaTime);
	bool getIsActive(){ return isActive;}
	void setIsActive(bool pActive){ isActive = pActive;}
	bool checkIfKeyIsReleased(char pChar);
	bool checkIfKeyIsPressed(char pChar);
	bool checkIfKeyIsDown(char pChar);
};
#endif // Movement_h__
