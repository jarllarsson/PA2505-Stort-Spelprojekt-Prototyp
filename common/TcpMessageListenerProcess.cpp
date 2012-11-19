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
	m_asyncDataLength = 0;
	m_asyncBufferSize = 100;
	m_asyncData = new char[m_asyncBufferSize];

	for(unsigned int i = 0; i < m_asyncBufferSize; i++)
	{
		m_asyncData[i] = 0;
	}

	boost::system::error_code error;

	m_activeSocket->async_receive(
		boost::asio::buffer( m_asyncData, m_asyncBufferSize ),
		boost::bind( &TcpMessageListenerProcess::handleReceive, this,
		error,
		boost::asio::placeholders::bytes_transferred));

	m_ioService->run_one();

	// HACK: should give sync problems... Doesn't work using mutex for some reason.
	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );
	}
//	// Blocking receive on socket.
//	m_asyncDataLength = m_activeSocket->receive( boost::asio::buffer(
//		m_asyncData, m_asyncBufferSize ) );
//
//	cout << "Received: " << m_asyncData << endl;
//	m_packetQueue.pushBack( string(m_asyncData) );
//
//	// Send back to server (for the lulz)
//	m_activeSocket->send( boost::asio::buffer(
//		m_asyncData, m_asyncDataLength ) );
//
//
//	// Don't do this here...
//	delete[] m_asyncData;

}

void TcpMessageListenerProcess::handleReceive(const boost::system::error_code& error,
		size_t bytes_transferred)
{
	cout << m_asyncData << endl;

	m_activeSocket->send( boost::asio::buffer(
		m_asyncData, m_asyncBufferSize ) );
}