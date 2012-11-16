#ifndef THREAD_SAFE_MESSAGING_H
#define THREAD_SAFE_MESSAGING_H

#include <boost/thread.hpp>
#include "ProcessMessage.h"
#include "QueueList.h"

class ThreadSafeMessaging
{
private:
	QueueList< ProcessMessage* > m_messageQueue;
	boost::mutex m_guard;

public:
	ThreadSafeMessaging();
	virtual ~ThreadSafeMessaging();

	// Sender gives away ownership of the message.
	void putMessage( ProcessMessage* p_event );
	
protected:
	// Receiver takes over ownership of the oldest message.
	ProcessMessage* popMessage();

	// Receiver takes over ownership of all messages.
	QueueList< ProcessMessage* > getMessages();


	unsigned int getMessagesAmount();

};

#endif