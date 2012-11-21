#include "HUD.h"
#include "Camera.h"
#include <sstream>

HUD::HUD(ID3D10Device* pDevice)
{
	device = pDevice;
	font = new Font(pDevice,24);
}
HUD::~HUD()
{
	delete font;
}
void HUD::draw()
{
	D3DXVECTOR3 pos = Camera::getInstance()->getPos();
	std::wstring cameraPos = L"";
	std::wostringstream outs; 
	outs.precision(6);
	outs << L"Camera: " << pos.x << ", " << pos.y << ", " << pos.z;
	cameraPos = outs.str();

	RECT drawRect = {5,5,0,0};

	font->drawText(drawRect,cameraPos);
}