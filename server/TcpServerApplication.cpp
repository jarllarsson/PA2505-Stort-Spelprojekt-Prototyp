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
	m_tcpListenerProcess = new TcpListenerProcess();
	m_tcpListenerProcess->start();
}

void TcpServerApplication::update()
{
	cout << "1";

	if( kbhit() )
	{
		if( _getch() == 27 )
			m_running = false;
	}

}