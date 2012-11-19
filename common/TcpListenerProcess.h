#ifndef TCP_LISTENER_PROCESS_H
#define TCP_LISTENER_PROCESS_H

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include "ProcessThread.h"
#include "ThreadSafeMessaging.h"
#include "ProcessMessage.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace boost::asio::ip;
using namespace std;

class TcpListenerProcess: public ProcessThread,
	public ThreadSafeMessaging
{
private:
	
	tcp::socket* m_socket;

	tcp::acceptor* m_acceptor;
	boost::asio::io_service* m_ioService;
	int m_port;

	ThreadSafeMessaging* m_parent;

	bool m_running;

public:
	TcpListenerProcess( ThreadSafeMessaging* p_parent, int p_port,
		boost::asio::io_service* p_ioService );
	~TcpListenerProcess();

	void body();

private:

	void startAccept();
	void handleAccept( const boost::system::error_code& p_error );


//	void acceptConnection( int p_port );
//	void sendWelcomeMessage( string p_message );

};

#endif