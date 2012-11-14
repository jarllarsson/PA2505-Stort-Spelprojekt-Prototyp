#include "TcpMessageListenerProcess.h"

TcpMessageListenerProcess::TcpMessageListenerProcess(
	tcp::socket* p_activeSocket,
	boost::asio::io_service* p_ioService )
{
	m_activeSocket = p_activeSocket;
	m_ioService = p_ioService;
}

TcpMessageListenerProcess::~TcpMessageListenerProcess()
{
}