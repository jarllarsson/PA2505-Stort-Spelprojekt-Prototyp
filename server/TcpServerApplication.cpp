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
}

void TcpServerApplication::run()
{
	m_running = true;

	init();

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

		 cout << "Message to TcpServerApplication: " <<
			message->message << endl;

		delete message;
	}


	if( _kbhit() )
	{
		if( _getch() == 27 )
		{
			m_running = false;
			m_tcpListenerProcessMessaging->putMessage(
				new ProcessMessage( "exit", this ) );
		}
	}
}