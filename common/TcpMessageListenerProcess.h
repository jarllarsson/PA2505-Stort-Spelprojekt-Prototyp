#ifndef TCP_MESSAGE_LISTENER_PROCESS_H
#define TCP_MESSAGE_LISTENER_PROCESS_H

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include "ProcessThread.h"

using namespace boost::asio::ip;
using namespace std;

class TcpMessageListenerProcess: public ProcessThread
{
private:
	tcp::socket* m_activeSocket;
	boost::asio::io_service* m_ioService;

public:
	TcpMessageListenerProcess( tcp::socket* p_activeSocket,
		boost::asio::io_service* p_ioService );
	virtual ~TcpMessageListenerProcess();

	void body();

};

#endif