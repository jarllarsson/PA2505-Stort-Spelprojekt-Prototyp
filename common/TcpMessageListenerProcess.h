#ifndef TCP_MESSAGE_LISTENER_PROCESS_H
#define TCP_MESSAGE_LISTENER_PROCESS_H

#include <boost\asio.hpp>

#include <string>
#include "QueueList.h"
#include "ProcessThread.h"

using namespace boost::asio::ip;
using namespace std;

class TcpMessageListenerProcess: public ProcessThread
{
private:
	tcp::socket* m_activeSocket;
	boost::asio::io_service* m_ioService;
	QueueList<string> m_packetQueue;

	char* m_asyncData;
	unsigned int m_asyncDataLength;
	unsigned int m_asyncBufferSize;

public:
	TcpMessageListenerProcess( tcp::socket* p_activeSocket,
		boost::asio::io_service* p_ioService );
	virtual ~TcpMessageListenerProcess();

	void body();

private:
//	void handleReceive( boost::system::error_code e );
	
	void handleReceive(const boost::system::error_code& error,
		size_t bytes_transferred);
};

#endif