#include "GameLogic.h"
#include "Globals.h"

GameLogic::GameLogic(ID3D10Device* pDevice)
{
	device = pDevice;
	SoundManager::getInstace()->init();
	
	int x,y,z,size;
	x = 30;
	y = 20;
	z = 0;
	size = 4;

	Box* newBox = new Box(device,D3DXVECTOR3(x,y,z),WHITE,1);
	SFX* sound = SoundManager::getInstace()->createSoundEffect(L"Sound//Helicopter.wav",newBox->getPos(),true);
	newBox->resolve(sound,'U',true);
	objects.push_back(newBox);
	
	newBox = new Box(device,D3DXVECTOR3(x-size,y,z+size),WHITE,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//Helicopter.wav",newBox->getPos(),true);
	newBox->resolve(sound,'U',true);
	objects.push_back(newBox);

	newBox = new Box(device,D3DXVECTOR3(x-size*2,y,z+size*2),WHITE,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//Helicopter.wav",newBox->getPos(),true);
	newBox->resolve(sound,'U',true);
	objects.push_back(newBox);

	newBox = new Box(device,D3DXVECTOR3(x+size,y,z+size),WHITE,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//Helicopter.wav",newBox->getPos(),true);
	newBox->resolve(sound,'U',true);
	objects.push_back(newBox);

	newBox = new Box(device,D3DXVECTOR3(x+size*2,y,z+size*2),WHITE,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//Helicopter.wav",newBox->getPos(),true);
	newBox->resolve(sound,'U',true);
	objects.push_back(newBox);

	newBox = new Box(device,D3DXVECTOR3(10,10,0),RED,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//AAAHHH_mono.wav",newBox->getPos(),true);
	newBox->resolve(sound,'I',false);
	objects.push_back(newBox);

	newBox = new Box(device,D3DXVECTOR3(-10,0,0),BLUE,1);
	sound = SoundManager::getInstace()->createSoundEffect(L"Sound//500Calibur.wav",newBox->getPos(),false);
	newBox->resolve(sound,'O',false);
	objects.push_back(newBox);
}

GameLogic::~GameLogic()
{
	SoundManager::destroy();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
}
void GameLogic::update(float pDeltaTime)
{
	InputHandler::getInstance()->update(pDeltaTime);
	SoundManager::getInstace()->update();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->update(pDeltaTime);
	}
	updateCamera();
}

void GameLogic::draw()
{
	for (unsigned int i = 0; i < objects.size();i++)
	{
		objects[i]->draw();
	}
}
void GameLogic::updateCamera()
{
	if (InputHandler::getInstance()->checkIfKeyIsPressed('W') || InputHandler::getInstance()->checkIfKeyIsDown('W'))
		Camera::getInstance()->setZVelocity(SPEEDZ);

	else if (InputHandler::getInstance()->checkIfKeyIsPressed('S') || InputHandler::getInstance()->checkIfKeyIsDown('S'))
		Camera::getInstance()->setZVelocity(-1*SPEEDZ);
	
	if (InputHandler::getInstance()->checkIfKeyIsPressed('D') || InputHandler::getInstance()->checkIfKeyIsDown('D'))
		Camera::getInstance()->setXVelocity(SPEEDX);
	
	else if (InputHandler::getInstance()->checkIfKeyIsPressed('A') || InputHandler::getInstance()->checkIfKeyIsDown('A'))
		Camera::getInstance()->setXVelocity(-1*SPEEDX);
}