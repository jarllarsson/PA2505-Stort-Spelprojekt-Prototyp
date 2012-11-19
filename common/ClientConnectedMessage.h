#ifndef CLIENT_CONNECTED_MESSAGE_H
#define CLIENT_CONNECTED_MESSAGE_H

#include "ProcessMessage.h"

struct ClientConnectedMessage: public ProcessMessage
{
public:
	ClientConnectedMessage()
	{
		type = MessageTypes::NEW_CLIENT;
	}



};

#endif