#include "ThreadSafeMessaging.h"

ThreadSafeMessaging::ThreadSafeMessaging()
{
}

ThreadSafeMessaging::~ThreadSafeMessaging()
{
	m_mutex.lock();

	while( m_eventQueue.length() > 0 )
	{
		if( m_eventQueue.front() )
		{
			delete m_eventQueue.front();
			m_eventQueue.popFront();
		}
	}

	m_mutex.unlock();
}

void ThreadSafeMessaging::sendEvent( ProcessEvent* p_event )
{
	m_mutex.lock();

	m_eventQueue.pushBack( p_event );

	m_mutex.unlock();
}