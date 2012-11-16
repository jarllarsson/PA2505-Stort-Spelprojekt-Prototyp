#include "TcpListenerProcess.h"


TcpListenerProcess::TcpListenerProcess( ThreadSafeMessaging* p_parent )
{
	m_running = false;
	m_socket = NULL;
	m_acceptor = NULL;
	m_parent = p_parent;
}

TcpListenerProcess::~TcpListenerProcess()
{
	delete m_acceptor;
	if( m_socket )
		delete m_socket;
}

void TcpListenerProcess::body()
{
	m_running = true;

	while( m_running )
	{

//		if( getMessagesAmount() > 0 )
//		{
//			QueueList< ProcessMessage* > messages;
//			messages = getMessages();
//
//			while( messages.length() > 0 )
//			{
//				ProcessMessage* message = messages.popFront();
//
//				if( message->message == "exit" )
//					m_running = false;
//
//				delete message;
//			}
//		}



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