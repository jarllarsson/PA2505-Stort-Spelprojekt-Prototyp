#ifndef TCP_LISTENER_PROCESS_H
#define TCP_LISTENER_PROCESS_H

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <ProcessThread.h>
#include <ThreadSafeMessaging.h>
#include <ProcessMessage.h>
#include <iostream>
#include <string>

using namespace boost::asio::ip;
using namespace std;

class TcpListenerProcess: public ProcessThread,
	public ThreadSafeMessaging
{
private:
	tcp::socket* m_socket;
	tcp::acceptor* m_acceptor;
	boost::asio::io_service m_ioService;

	ThreadSafeMessaging* m_parent;

	bool m_running;

public:
	TcpListenerProcess( ThreadSafeMessaging* p_parent );
	~TcpListenerProcess();

	void body();

private:
	void acceptConnection( int p_port );
	void sendWelcomeMessage( string p_message );

};

#endif