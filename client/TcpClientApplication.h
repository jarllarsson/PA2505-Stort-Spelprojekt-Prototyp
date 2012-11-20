#ifndef TCP_CLIENT_APPLICATION_H
#define TCP_CLIENT_APPLICATION_H

#include <string>
#include <iostream>
#include <conio.h>

#include <boost/asio.hpp>

#include <ThreadSafeMessaging.h>
#include <ProcessMessage.h>
#include <TcpMessengerProcess.h>

using namespace std;
using namespace boost::asio::ip;

class TcpClientApplication: public ThreadSafeMessaging
{
private:
	boost::asio::io_service* m_ioService;
	TcpMessengerProcess* m_messengerProcess;

	tcp::socket* m_activeSocket;

	bool m_running;

public:
	TcpClientApplication();
	~TcpClientApplication();

	void run();

	// HACK: mby delete this? I think it won't be called... ever.
	void stop();

private:
	void init();
	void update();

	// HACK: should probably be placed in a different process/thread.
	bool connect( string p_ip, string p_port );


};

#endif