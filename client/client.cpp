#include <iostream>
#include <conio.h>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <TcpMessageListenerProcess.h>

using namespace std;
using namespace boost::asio::ip;


int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// io service:
	boost::asio::io_service ioService;
	


	// resolver:
	tcp::resolver tcpResolver( ioService );
	tcp::resolver::query tcpQuery( "127.0.0.1", "1337" );

	tcp::resolver::iterator endpointIterator;
	tcp::resolver::iterator end;

	endpointIterator = tcpResolver.resolve( tcpQuery );
	end = tcp::resolver::iterator();



	// socket:
	tcp::socket socket( ioService );

	boost::system::error_code error;
	error = boost::asio::error::host_not_found;

	// iterate and attempt to connect to resolved endpoints
	while( error && endpointIterator != end )
	{
		socket.close();
		socket.connect( *endpointIterator, error );

		*endpointIterator++;
	}

	if( error )
	{
		throw boost::system::system_error( error );
	}
	else
	{
		tcp::no_delay option( true );
		socket.set_option( option );
		tcp::socket::non_blocking_io nonBlocking( true );
		socket.io_control( nonBlocking );

		ProcessThread* messageListener = new TcpMessageListenerProcess( &socket, &ioService );
		messageListener->start();

		while( true )
		{
			char inputCharacter = _getch();

			if( inputCharacter == 'q' )
			{
				ioService.stop();
				break;
			}
		}

		messageListener->stop();
	}

	return 0;
}