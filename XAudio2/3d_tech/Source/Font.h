#ifndef Font_h__
#define Font_h__
#include <D3DX10.h>
#include <string>

class Font
{
private:
	ID3DX10Font* font;
	int offset;
	int fontSize;
protected:
	
public:
	Font(ID3D10Device* device,int pFontSize);
	~Font();
	void drawText(RECT R, std::wstring pText);
};

#endif // Font_h__