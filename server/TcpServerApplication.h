#ifndef TCP_SERVER_APPLICATION_H
#define TCP_SERVER_APPLICATION_H

#include <vector>
#include <conio.h>
#include <typeinfo>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <ThreadSafeMessaging.h>
#include <QueueList.h>
#include <ProcessMessage.h>

#include "TcpListenerProcess.h"

using namespace std;
using namespace boost::asio::ip;

class TcpServerApplication: public ThreadSafeMessaging
{
private:
	vector< tcp::socket* > m_clientSockets; // Replace this with "client-processes".

	bool m_running;

	ProcessThread* m_tcpListenerProcess;
	ThreadSafeMessaging* m_tcpListenerProcessMessaging;


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