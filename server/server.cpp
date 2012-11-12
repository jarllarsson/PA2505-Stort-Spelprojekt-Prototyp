#include <iostream>
#include <vector>

using namespace std;

#include <boost\asio.hpp>
#include <boost\array.hpp>

using namespace boost::asio::ip;

#include "TcpListenerProcess.h"

boost::mutex mutex;

void acceptFunction( boost::asio::io_service* p_ioService,
	tcp::acceptor *p_acceptor,
	vector<tcp::socket>* p_sockets )
{
	// socket:
	tcp::socket socket( *p_ioService );

	// block for connection
	p_acceptor->accept( socket );

	
	
	mutex.lock();

	

	mutex.unlock();
}

int main()
{
	vector<tcp::socket> sockets;

	// io service:
	boost::asio::io_service ioService;

	// acceptor
	tcp::acceptor acceptor( ioService, tcp::endpoint(tcp::v4(), 1337) );

	boost::thread acceptThread( &acceptFunction, &ioService, &acceptor, &sockets );
	acceptThread.join();

	cout << "accepted";

	ProcessThread* proc = new TcpListenerProcess();
	proc->start();

	std::cin.get();

	proc->stop();
	delete proc;

	return 0;
}