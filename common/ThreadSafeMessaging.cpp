#include "ThreadSafeMessaging.h"

ThreadSafeMessaging::ThreadSafeMessaging()
{
}

ThreadSafeMessaging::~ThreadSafeMessaging()
{
	m_guard.lock();

	while( m_messageQueue.length() > 0 )
	{
		if( m_messageQueue.front() )
		{
			delete m_messageQueue.front();
			m_messageQueue.popFront();
		}
	}

	m_guard.unlock();
}

void ThreadSafeMessaging::putMessage( ProcessMessage* p_message )
{
	if( p_message )
	{
		m_guard.lock();

		m_messageQueue.pushBack( p_message );

		m_guard.unlock();
	}
}

ProcessMessage* ThreadSafeMessaging::popMessage()
{
	ProcessMessage* message = NULL;

	m_guard.lock();


	message = m_messageQueue.popFront();

	m_guard.unlock();


	return message;
}

QueueList< ProcessMessage* > ThreadSafeMessaging::getMessages()
{
	QueueList< ProcessMessage* > messages;


	m_guard.lock();

	messages = m_messageQueue;
	m_messageQueue.clear();

	m_guard.unlock();


	return messages;
}

unsigned int ThreadSafeMessaging::getMessagesAmount()
{
	unsigned int messagesAmount = 0;


	m_guard.lock();

	messagesAmount = m_messageQueue.length();
	
	m_guard.unlock();


	return messagesAmount;
}