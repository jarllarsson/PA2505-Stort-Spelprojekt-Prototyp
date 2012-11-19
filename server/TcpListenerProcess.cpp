#include "TcpListenerProcess.h"


TcpListenerProcess::TcpListenerProcess( ThreadSafeMessaging* p_parent, int p_port,
	boost::asio::io_service* p_ioService )
{
	m_running = false;
	m_socket = NULL;
	m_acceptor = NULL;
	m_parent = p_parent;
	m_port = p_port;
	m_ioService = p_ioService;

	m_acceptor = new tcp::acceptor( *m_ioService, tcp::endpoint( tcp::v4(), p_port ) );
}

TcpListenerProcess::~TcpListenerProcess()
{
	delete m_acceptor;

	m_ioService->stop();

	if( m_socket )
		delete m_socket;

}

void TcpListenerProcess::body()
{
	m_running = true;

	startAccept();

	cout << "TcpListenerProcess: Started listening on port " << m_port << ".\n";

	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );

		// Poll for new connections.
		m_ioService->poll();

		while( getMessagesAmount() > 0 )
		{
			ProcessMessage* message = popMessage();

			if( message->type == MessageType::TERMINATE )
				 m_running = false;

			delete message;
		}

	}

}

void TcpListenerProcess::startAccept()
{
	m_ioService->reset();

	m_socket = new tcp::socket( *m_ioService );
	
	tcp::acceptor::non_blocking_io nonBlocking( true );
	m_acceptor->io_control( nonBlocking );

	m_acceptor->async_accept( *m_socket,
		boost::bind( &TcpListenerProcess::handleAccept, this) );
	
}

void TcpListenerProcess::handleAccept()
{
	m_parent->putMessage( new ProcessMessage(
		MessageType::NEW_CLIENT, this, m_socket ) );

	m_socket = NULL;


	// After call-back is handled, a new callback should be started.
	startAccept();
}









//void TcpListenerProcess::acceptConnection(int p_port)
//{
//	
//	m_acceptor = new tcp::acceptor( m_ioService,
//		tcp::endpoint( tcp::v4(), p_port ) );
//
//	m_socket = new tcp::socket( m_ioService );
//
//	m_acceptor->accept( *m_socket );
//
//
//}
//
//void TcpListenerProcess::sendWelcomeMessage( string p_message )
//{
//	m_socket->send( boost::asio::buffer( p_message ) );
//}