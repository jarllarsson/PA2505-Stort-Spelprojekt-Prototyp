#ifndef TCP_LISTENER_PROCESS_H
#define TCP_LISTENER_PROCESS_H

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <ProcessThread.h>
#include <iostream>
#include <string>

using namespace boost::asio::ip;
using namespace std;

class TcpListenerProcess: public ProcessThread
{
private:
	tcp::socket* m_socket;
	tcp::acceptor* m_acceptor;
	boost::asio::io_service m_ioService;


public:
	TcpListenerProcess();
	~TcpListenerProcess();

	void body();

private:
	void acceptConnection( int p_port );
	void sendWelcomeMessage( string p_message );

};

#endif