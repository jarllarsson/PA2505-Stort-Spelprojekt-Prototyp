#include <iostream>
#include <vector>
#include <conio.h>

using namespace std;

#include "TcpServerApplication.h"
#include "TcpListenerProcess.h"

int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	TcpServerApplication app;

	app.run();



	return 0;
}