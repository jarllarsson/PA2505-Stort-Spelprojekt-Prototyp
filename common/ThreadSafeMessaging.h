#ifndef THREAD_SAFE_MESSAGING_H
#define THREAD_SAFE_MESSAGING_H

#include <boost/thread.hpp>
#include "ProcessEvent.h"
#include "QueueList.h"

class ThreadSafeMessaging
{
private:
	QueueList< ProcessEvent* > m_eventQueue;
	boost::mutex m_mutex;

public:
	ThreadSafeMessaging();
	virtual ~ThreadSafeMessaging();

	// Sender gives away ownership of the message.
	void sendEvent( ProcessEvent* p_event );
	
protected:
	// Receiver takes over ownership of the message.
	ProcessEvent* popEvent();

};

#endif