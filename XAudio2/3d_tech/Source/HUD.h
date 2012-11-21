#ifndef HUD_h__
#define HUD_h__
#include <D3D10.h>
#include <D3DX10.h>
#include "Font.h"
#include <string>

class HUD
{
private:
	ID3D10Device* device;
	int offset;
	Font* font;
protected:
	
public:
	HUD(ID3D10Device* pDevice);
	~HUD();
	void draw();
};

#endif // HUD_h__