#ifndef TCP_SERVER_APPLICATION_H
#define TCP_SERVER_APPLICATION_H

#include <vector>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include <ThreadSafeMessaging.h>

using namespace std;
using namespace boost::asio::ip;

class TcpServerApplication
{
protected:
	vector< tcp::socket* > m_clientSockets;

public:
	TcpServerApplication();
	~TcpServerApplication();
	
	void run();
	void stop();

private:
	void update();

};

#endif