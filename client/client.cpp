#include <iostream>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <TcpMessageListenerProcess.h>

using namespace std;
using namespace boost::asio::ip;


int main()
{
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
		ProcessThread* messageListener = new TcpMessageListenerProcess( &socket, &ioService );
		messageListener->start();

		cin.get();

		messageListener->interrupt();
	}



	cin.get();

	return 0;
}