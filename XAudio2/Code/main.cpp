#include "stdafx.h"
#include "RenderWindow.h"
#include <time.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	srand( time(NULL));
	RenderWindow* window = new RenderWindow();
	window->Init( hInstance, nCmdShow );
	window->Run();
	delete window;
}