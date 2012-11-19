#include "TcpMessengerProcess.h"

TcpMessengerProcess::TcpMessengerProcess(
	tcp::socket* p_activeSocket,
	boost::asio::io_service* p_ioService )
{
	m_activeSocket = p_activeSocket;
	m_ioService = p_ioService;
}

TcpMessengerProcess::~TcpMessengerProcess()
{
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
		error,
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

void TcpMessengerProcess::handleReceive(const boost::system::error_code& error,
		size_t bytes_transferred)
{
	cout << m_asyncData << endl;

	m_activeSocket->send( boost::asio::buffer(
		m_asyncData, m_asyncBufferSize ) );
}