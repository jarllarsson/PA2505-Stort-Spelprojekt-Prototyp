#include "TcpListenerProcess.h"


TcpListenerProcess::TcpListenerProcess( ThreadSafeMessaging* p_parent, int p_port )
{
	m_running = false;
	m_socket = NULL;
	m_acceptor = NULL;
	m_parent = p_parent;

	m_acceptor = new tcp::acceptor( m_ioService,
		tcp::endpoint( tcp::v4(), p_port ) );
}

TcpListenerProcess::~TcpListenerProcess()
{
	delete m_acceptor;

	if( m_socket )
		delete m_socket;

	m_ioService.stop();
}

void TcpListenerProcess::body()
{
	m_running = true;

	startAccept();

	while( m_running )
	{
		m_ioService.poll();

		while( getMessagesAmount() > 0 )
		{
			ProcessMessage* message = popMessage();

			 if( message->message == "exit" )
				 m_running = false;

			delete message;
		}

	}


	//	cout << "Hello from within the TcpListenerProcess' body\n";
	//
	//	acceptConnection( 1337 );
	//
	//	cout << "Accepted.\n";
	//
	//	sendWelcomeMessage( "*WelcomeMessage*\n" );

}

void TcpListenerProcess::startAccept()
{
	m_ioService.reset();

	m_socket = new tcp::socket( m_ioService );
	
	tcp::acceptor::non_blocking_io nonBlocking( true );
	m_acceptor->io_control( nonBlocking );

	m_acceptor->async_accept( *m_socket,
		boost::bind( &TcpListenerProcess::handleAccept, this) );
	
}

void TcpListenerProcess::handleAccept()
{
	if( true )
	{
		stringstream ss;
		ss << "new_client_";
		ss << m_socket->local_endpoint().address().to_string();

		string msg = ss.str();

		m_parent->putMessage( new ProcessMessage(
			msg, this ) );

//		cout << msg << endl;
	}


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