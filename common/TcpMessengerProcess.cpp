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


	tcp::no_delay option( true );
	m_activeSocket->set_option( option );
	tcp::socket::non_blocking_io nonBlocking( true );
	m_activeSocket->io_control( nonBlocking );


	startReceive();



	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );

		while( getMessagesAmount() > 0 )
		{
			m_ioService->poll();

			ProcessMessage* message = popMessage();

			if( message->type == MessageType::TERMINATE )
			{
				 m_running = false;
			}
			else if( message->type == MessageType::SEND_PACKET )
			{
				cout << "Send: '" << message->message << "'.\n";

				m_activeSocket->send( boost::asio::buffer( message->message ) );

				//m_activeSocket->async_send(
				//	boost::asio::buffer( message->message ),
				//	boost::bind( &TcpMessengerProcess::handleSend, this,
				//	boost::asio::placeholders::error,
				//	boost::asio::placeholders::bytes_transferred ) );

			}

			delete message;
		}

	}
}

void TcpMessengerProcess::startReceive()
{
	m_ioService->reset();

	m_activeSocket->async_receive(
		boost::asio::buffer( m_asyncData, m_asyncBufferSize ),
		boost::bind( &TcpMessengerProcess::handleReceive, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

}

void TcpMessengerProcess::handleReceive( const boost::system::error_code& p_error,
		size_t p_bytesTransferred )
{
	if( p_error == boost::asio::error::eof )
	{
		// Connection closed cleanly. // TEST THIS!!
		m_parent->putMessage( new ProcessMessage(
			MessageType::CLIENT_DISCONNECTED, this, "clean_disconnect" ) );

		m_running = false;
	}
	else if( p_error == boost::asio::error::connection_reset )
	{
		// (Connection aborted)

		m_parent->putMessage( new ProcessMessage(
			MessageType::CLIENT_DISCONNECTED, this, "connection_abort" ) );

		m_running = false;

	}
	else if( p_error )
	{
		
	}
	else
	{
		if( p_bytesTransferred > 0 )
		{
			m_parent->putMessage( new ProcessMessage(
				MessageType::RECEIVE_PACKET,
				this,
				m_asyncData ) );

			startReceive();
		}
	}

}

void TcpMessengerProcess::handleSend( const boost::system::error_code& p_error,
		size_t p_bytesTransferred )
{
	// Do nothing when packet is sent.
}