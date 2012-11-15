#ifndef TCP_SERVER_APPLICATION_H
#define TCP_SERVER_APPLICATION_H

#include <vector>
#include <conio.h>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <ThreadSafeMessaging.h>
#include "TcpListenerProcess.h"

using namespace std;
using namespace boost::asio::ip;

class TcpServerApplication: public ThreadSafeMessaging
{
private:
	vector< tcp::socket* > m_clientSockets;
	bool m_running;
	ProcessThread* m_tcpListenerProcess;

public:
	TcpServerApplication();
	~TcpServerApplication();
	
	void run();
	void stop();

private:
	void init();
	void update();

};

#endif