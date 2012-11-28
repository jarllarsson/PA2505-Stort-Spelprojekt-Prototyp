#ifndef TCP_MESSENGER_PROCESS_H
#define TCP_MESSENGER_PROCESS_H

#include <string>

#include <boost/asio.hpp>

#include "ThreadSafeMessaging.h"
#include "QueueList.h"
#include "ProcessThread.h"

using namespace boost::asio::ip;
using namespace std;

class TcpMessengerProcess: public ProcessThread,
	public ThreadSafeMessaging
{
private:
	tcp::socket* m_activeSocket;
	boost::asio::io_service* m_ioService;

	ThreadSafeMessaging* m_parent;

	QueueList<string> m_packetQueue;

	char* m_asyncData;
	unsigned int m_asyncDataLength;
	unsigned int m_asyncBufferSize;
	
public:
	TcpMessengerProcess( ThreadSafeMessaging* p_parent,
		tcp::socket* p_activeSocket,
		boost::asio::io_service* p_ioService );
	virtual ~TcpMessengerProcess();

	void body();

private:
	void startReceive();

	void handleReceive( const boost::system::error_code& p_error,
		size_t p_bytesTransferred );

	void handleSend( const boost::system::error_code& p_error,
		size_t p_bytesTransferred );

};

#endif