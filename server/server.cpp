#include <iostream>
#include <vector>

using namespace std;

#include "TcpListenerProcess.h"

int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	ProcessThread* proc = new TcpListenerProcess();
	proc->start();

	std::cin.get();

	proc->stop();
	delete proc;

	return 0;
}