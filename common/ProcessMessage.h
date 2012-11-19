#ifndef PROCESS_MESSAGE_H
#define PROCESS_MESSAGE_H

#include <string>
#include <boost/asio.hpp>

using namespace std;

class ThreadSafeMessaging;

struct MessageType
{
	enum MESSAGE_TYPE{
		NONE,
		TEXT,
		NEW_CLIENT,
		TERMINATE,
		NEW_PACKET
	};
};

struct ProcessMessage
{
public:
	MessageType::MESSAGE_TYPE type;
	ThreadSafeMessaging* sender;


	// HACK: These should be "modular".
	string message;
	boost::asio::ip::tcp::socket* socket;


	ProcessMessage()
	{
		message = "";
		sender = NULL;
	}

	ProcessMessage( MessageType::MESSAGE_TYPE p_type, ThreadSafeMessaging* p_sender,
		string p_message )
	{
		type = p_type;
		message = p_message;
		sender = p_sender;
	}

	ProcessMessage( MessageType::MESSAGE_TYPE p_type, ThreadSafeMessaging* p_sender,
		boost::asio::ip::tcp::socket* p_socket )
	{
		type = p_type;
		sender = p_sender;
		socket = p_socket;
	}

};

#endif