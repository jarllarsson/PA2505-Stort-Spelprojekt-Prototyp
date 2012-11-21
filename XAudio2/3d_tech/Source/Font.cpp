#include "Font.h"

Font::Font(ID3D10Device* device,int pFontSize)
{
	offset = 0;
	fontSize = pFontSize;
	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = fontSize;
	fontDesc.Width           = 0;
	fontDesc.Weight          = 0;
	fontDesc.MipLevels       = 1;
	fontDesc.Italic          = true;
	fontDesc.CharSet         = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality         = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy_s(fontDesc.FaceName, L"Arial");

	D3DX10CreateFontIndirect(device, &fontDesc, &font);
}
Font::~Font()
{
	font->Release();
}

void Font::drawText(RECT R, std::wstring pText)
{
	D3DXCOLOR lWhite = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	font->DrawText(0,pText.c_str(),-1,&R,DT_NOCLIP,lWhite);
}