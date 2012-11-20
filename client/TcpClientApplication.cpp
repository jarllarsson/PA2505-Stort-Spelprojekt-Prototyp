#include "TcpClientApplication.h"

TcpClientApplication::TcpClientApplication()
{
	m_running = false;
	m_ioService = NULL;
	m_messengerProcess = NULL;
	m_activeSocket = NULL;
}

TcpClientApplication::~TcpClientApplication()
{

	if( m_messengerProcess )
	{
		m_messengerProcess->putMessage( new ProcessMessage(
			MessageType::TERMINATE, this, "see you" ) );

		m_messengerProcess->stop();
		delete m_messengerProcess;
	}

	
	if( m_ioService )
		delete m_ioService;
}

void TcpClientApplication::run()
{
	m_running = true;

	init();

	bool connected = false;

	do
	{
		string inputIp = "";
		cout << "Enter ip (or 'exit' to quit): ";
		getline( cin, inputIp );

		if( inputIp == "exit")
			return;
		else if( inputIp == "" )
			inputIp = "localhost";

		string inputPort = "";
		cout << "Enter port: ";
		getline( cin, inputPort );

		if( inputPort == "" )
			inputPort = "1337";
		cin.clear();

		connected = connect( inputIp, inputPort );


	} while( !connected );


	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );
		update();
	}
}

void TcpClientApplication::update()
{
	while( getMessagesAmount() > 0 )
	{
		ProcessMessage* message = popMessage();

		

		delete message;
	}
	

	if( _kbhit() )
	{
		char inputKey = _getch();

		if( inputKey == 27 )
		{
			m_running = false;
		}
		else if( inputKey == 'm' )
		{
			m_messengerProcess->putMessage( new ProcessMessage(
				MessageType::SEND_PACKET, this, "lol" ) );
		}

	}
}


void TcpClientApplication::stop()
{
	m_running = false;
}

void TcpClientApplication::init()
{
	m_ioService = new boost::asio::io_service();

}

bool TcpClientApplication::connect( string p_ip, string p_port )
{
	// resolver:
	tcp::resolver tcpResolver( *m_ioService );
	tcp::resolver::query tcpQuery( p_ip, p_port );

	tcp::resolver::iterator endpointIterator;
	tcp::resolver::iterator end;

	endpointIterator = tcpResolver.resolve( tcpQuery );
	end = tcp::resolver::iterator();


	// socket:
	m_activeSocket = new tcp::socket( *m_ioService );


	boost::system::error_code error;
	error = boost::asio::error::host_not_found;

	// iterate and attempt to connect to resolved endpoints
	while( error && endpointIterator != end )
	{
		m_activeSocket->close();
		m_activeSocket->connect( *endpointIterator, error );

		*endpointIterator++;
	}

	if( error )
	{
		cout << error.message() << endl;
		cin.get();
		return false;

//		throw boost::system::system_error( error );
	}
	else
	{
		tcp::no_delay option( true );
		m_activeSocket->set_option( option );

		tcp::socket::non_blocking_io nonBlocking( true );
		m_activeSocket->io_control( nonBlocking );

		m_messengerProcess = new TcpMessengerProcess(
			this, m_activeSocket, m_ioService );
		m_messengerProcess->start();

	}

	return true;
}