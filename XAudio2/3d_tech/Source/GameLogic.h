#ifndef GameLogic_h__
#define GameLogic_h__
#include "InputHandler.h"
#include "SoundManager.h"
#include "Object.h"
#include <vector>
#include "Box.h"
#include <d3d10.h>

using namespace std;

class GameLogic
{
private:

	const static int SPEEDZ = 9;
	const static int SPEEDX = 9;

	bool isActive;
	vector<Object*> objects;
	ID3D10Device* device;
private:
	void updateCamera();
protected:
public:
	GameLogic(ID3D10Device* pDevice);
	~GameLogic();
	void update(float pDeltaTime);
	void draw();
	void playSound();
	bool getIsActive(){ return isActive;}
	void setIsActive(bool pActive){ isActive = pActive;}
};
#endif // GameLogic_h__
