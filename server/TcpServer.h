#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <vector>
using namespace std;

#include <boost\asio.hpp>
#include <boost\array.hpp>
using namespace boost::asio::ip;

class TcpServer
{
protected:
	vector<tcp::socket> m_sockets;


public:


};

#endif