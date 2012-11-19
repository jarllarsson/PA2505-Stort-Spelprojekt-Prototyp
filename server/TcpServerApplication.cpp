#include "TcpServerApplication.h"

TcpServerApplication::TcpServerApplication()
{
	m_running = false;
	m_ioService = NULL;
}

TcpServerApplication::~TcpServerApplication()
{
	if( m_tcpListenerProcess )
	{
		m_tcpListenerProcess->stop();
		delete m_tcpListenerProcess;
	}

	for(unsigned int i = 0; i < m_messengerProcesses.size(); i++)
	{
		if( m_messengerProcesses[i] )
		{
			m_messengerProcesses[i]->putMessage(
				new ProcessMessage( MessageType::TERMINATE, this, "byebye" ) );
			m_messengerProcesses[i]->stop();
			delete m_messengerProcesses[i];
		}
	}
	m_messengerProcesses.clear();


	if( m_ioService )
		delete m_ioService;
}

void TcpServerApplication::run()
{
	m_running = true;

	init();

	cout << "Server initialized.\n";

	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );
		update();
	}

}

void TcpServerApplication::stop()
{
	m_running = false;
}

void TcpServerApplication::init()
{
	m_ioService = new boost::asio::io_service();

	m_tcpListenerProcess = new TcpListenerProcess( this, 1337, m_ioService );
	m_tcpListenerProcess->start();

	m_tcpListenerProcessMessaging = 
		dynamic_cast<ThreadSafeMessaging*>(m_tcpListenerProcess);
}

void TcpServerApplication::update()
{

	while( getMessagesAmount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::CLIENT_CONNECTED )
		{
//			m_clientSockets.push_back( message->socket );
//
			cout << "Client connected: " <<
				message->socket->local_endpoint().address().to_string() << ".\n";
//
//			message->socket->send( boost::asio::buffer( "Hello!" ) );








			TcpMessengerProcess* messengerProcess =
				new TcpMessengerProcess( this, message->socket, m_ioService );

			messengerProcess->start();

			messengerProcess->putMessage( new ProcessMessage(
				MessageType::NEW_PACKET, this, "Hello_1234" ) );

			m_messengerProcesses.push_back( messengerProcess );
		}
		else if( message->type == MessageType::CLIENT_DISCONNECTED )
		{
			cout << "Disconnected!\n";
		}


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