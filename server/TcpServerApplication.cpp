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
	m_tcpListenerProcess = new TcpListenerProcess( this );
	m_tcpListenerProcess->start();

	m_tcpListenerProcessMessaging = 
		dynamic_cast<ThreadSafeMessaging*>(m_tcpListenerProcess);
}

void TcpServerApplication::update()
{
	if( getMessagesAmount() > 0 )
	{
		 QueueList< ProcessMessage* > messages;
		 messages = getMessages();

		 while( messages.length() > 0 )
		 {
			 ProcessMessage* message = messages.popFront();

			 cout << "Message to TcpServerApplication: " <<
				 message->message << endl;

			 delete message;
		 }
	}

	if( kbhit() )
	{
		if( _getch() == 27 )
		{
			m_running = false;
			m_tcpListenerProcessMessaging->putMessage(
				new ProcessMessage( "exit", this ) );
		}
	}
}