#include "ThreadSafeMessaging.h"

ThreadSafeMessaging::ThreadSafeMessaging()
{
}

ThreadSafeMessaging::~ThreadSafeMessaging()
{
	m_mutex.lock();

	while( m_messageQueue.length() > 0 )
	{
		if( m_messageQueue.front() )
		{
			delete m_messageQueue.front();
			m_messageQueue.popFront();
		}
	}

	m_mutex.unlock();
}

void ThreadSafeMessaging::putMessage( ProcessMessage* p_message )
{
	if( p_message )
	{
		m_mutex.lock();

		m_messageQueue.pushBack( p_message );

		m_mutex.unlock();
	}
}

ProcessMessage* ThreadSafeMessaging::popMessage()
{
	ProcessMessage* message = NULL;

	m_mutex.lock();


	message = m_messageQueue.popFront();

	m_mutex.unlock();


	return message;
}

QueueList< ProcessMessage* > ThreadSafeMessaging::getMessages()
{
	QueueList< ProcessMessage* > messages;


	m_mutex.lock();

	messages = m_messageQueue;

	m_mutex.unlock();


	return messages;
}

unsigned int ThreadSafeMessaging::getMessagesAmount()
{
	unsigned int messagesAmount = 0;


	m_mutex.lock();

	messagesAmount = m_messageQueue.length();
	
	m_mutex.unlock();


	return messagesAmount;
}