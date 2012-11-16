#ifndef PROCESS_MESSAGE_H
#define PROCESS_MESSAGE_H

#include <string>
using namespace std;

class ThreadSafeMessaging;

class ProcessMessage
{
public:
	string message; // Let's go with string at first.

	ThreadSafeMessaging* sender;

	ProcessMessage()
	{
		message = "";
		sender = NULL;
	}

	ProcessMessage( string p_message, ThreadSafeMessaging* p_sender )
	{
		message = p_message;
		sender = p_sender;
	}
};

#endif