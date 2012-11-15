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

//	ProcessThread* proc = new TcpListenerProcess();
//	proc->start();
//
//	while( true )
//	{
//		int inputCharacter = _getch();
//		if( inputCharacter == 'q' )
//		{
//			break;
//		}
//	}
//
//	proc->stop();
//	delete proc;

	return 0;
}