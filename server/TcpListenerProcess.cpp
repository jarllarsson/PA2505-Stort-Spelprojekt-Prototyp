#include "TcpListenerProcess.h"


TcpListenerProcess::TcpListenerProcess()
{
	m_socket = NULL;
	m_acceptor = NULL;

}

TcpListenerProcess::~TcpListenerProcess()
{
	delete m_acceptor;
	if( m_socket )
		delete m_socket;
}

void TcpListenerProcess::body()
{

//	cout << "Hello from within the TcpListenerProcess' body\n";
//
//	acceptConnection( 1337 );
//
//	cout << "Accepted.\n";
//
//	sendWelcomeMessage( "*WelcomeMessage*\n" );
}

void TcpListenerProcess::acceptConnection(int p_port)
{
	
	m_acceptor = new tcp::acceptor( m_ioService,
		tcp::endpoint( tcp::v4(), p_port ) );

	m_socket = new tcp::socket( m_ioService );

	m_acceptor->accept( *m_socket );
}

void TcpListenerProcess::sendWelcomeMessage( string p_message )
{
	m_socket->send( boost::asio::buffer( p_message ) );
}