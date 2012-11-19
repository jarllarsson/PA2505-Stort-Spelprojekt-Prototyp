#include "TcpMessengerProcess.h"

TcpMessengerProcess::TcpMessengerProcess(
	ThreadSafeMessaging* p_parent,
	tcp::socket* p_activeSocket,
	boost::asio::io_service* p_ioService )
{
	m_parent = p_parent;
	m_activeSocket = p_activeSocket;
	m_ioService = p_ioService;
}

TcpMessengerProcess::~TcpMessengerProcess()
{
	m_activeSocket->close();
	delete m_activeSocket;
	delete[] m_asyncData;
}

void TcpMessengerProcess::body()
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
		boost::bind( &TcpMessengerProcess::handleReceive, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	m_ioService->run_one();



	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );

		while( getMessagesAmount() > 0 )
		{
			ProcessMessage* message = popMessage();

			if( message->type == MessageType::TERMINATE )
				 m_running = false;

			delete message;
		}

	}
}

void TcpMessengerProcess::handleReceive( const boost::system::error_code& error,
		size_t p_bytesTransferred )
{
	if( error == boost::asio::error::eof )
	{
		// Connection closed cleanly. // TEST THIS!!
	}
	else if( error == boost::asio::error::connection_reset )
	{
		// (Connection aborted)

		m_parent->putMessage( new ProcessMessage(
			MessageType::CLIENT_DISCONNECTED, this, "aborted" ) );

		m_running = false;

	}
	else if( error )
	{
		throw boost::system::system_error( error );
	}
	else
	{
		if( p_bytesTransferred > 0 )
		{
			cout << m_asyncData << endl;
		}
	}

}