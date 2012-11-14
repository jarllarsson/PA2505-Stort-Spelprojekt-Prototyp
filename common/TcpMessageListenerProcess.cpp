#include "TcpMessageListenerProcess.h"

TcpMessageListenerProcess::TcpMessageListenerProcess(
	tcp::socket* p_activeSocket,
	boost::asio::io_service* p_ioService )
{
	m_activeSocket = p_activeSocket;
	m_ioService = p_ioService;
}

TcpMessageListenerProcess::~TcpMessageListenerProcess()
{
}

void TcpMessageListenerProcess::body()
{
	unsigned int length = 0;
	unsigned int bufferSize = 100;
	char* data = new char[bufferSize];

	for(unsigned int i = 0; i < bufferSize; i++)
	{
		data[i] = 0;
	}

	// Blocking receive on socket.
	length = m_activeSocket->receive( boost::asio::buffer(
		data, bufferSize ) );

	cout << "Received: " << data << endl;

	delete[] data;
}