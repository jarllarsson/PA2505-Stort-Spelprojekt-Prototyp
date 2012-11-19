#include "TcpServerApplication.h"

TcpServerApplication::TcpServerApplication()
{
	m_running = false;
}

TcpServerApplication::~TcpServerApplication()
{
	if( m_tcpListenerProcess )
	{
		m_tcpListenerProcess->stop();
		delete m_tcpListenerProcess;
	}

	for(unsigned int i = 0; i < m_clientSockets.size(); i++)
	{
		if( m_clientSockets[i] )
		{
			delete m_clientSockets[i];
		}
	}
	m_clientSockets.clear();
}

void TcpServerApplication::run()
{
	m_running = true;

	init();

	cout << "Server initialized.\n";

	while( m_running )
	{
		update();
	}

}

void TcpServerApplication::stop()
{
	m_running = false;
}

void TcpServerApplication::init()
{
	m_tcpListenerProcess = new TcpListenerProcess( this, 1337 );
	m_tcpListenerProcess->start();

	m_tcpListenerProcessMessaging = 
		dynamic_cast<ThreadSafeMessaging*>(m_tcpListenerProcess);
}

void TcpServerApplication::update()
{

	while( getMessagesAmount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::NEW_CLIENT )
		{
			m_clientSockets.push_back( message->socket );

			cout << "Client connected: " <<
				message->socket->local_endpoint().address().to_string() << ".\n";
		}

//		cout << "Message to TcpServerApplication: " <<
//			message->message << endl;

		delete message;
	}


	if( _kbhit() )
	{
		if( _getch() == 27 )
		{
			m_running = false;
			m_tcpListenerProcessMessaging->putMessage(
				new ProcessMessage( MessageType::TERMINATE, this, "exit" ) );
		}
	}
}