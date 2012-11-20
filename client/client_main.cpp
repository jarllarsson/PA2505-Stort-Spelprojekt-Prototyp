#include <iostream>
#include <conio.h>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include "TcpClientApplication.h"
#include <TcpMessengerProcess.h>

using namespace std;
using namespace boost::asio::ip;


int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	TcpClientApplication app;

	app.run();

	return 0;
}