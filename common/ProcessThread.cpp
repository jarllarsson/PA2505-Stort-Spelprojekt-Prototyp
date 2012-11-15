#include "ProcessThread.h"

int ProcessThread::nextId = 0;

ProcessThread::ProcessThread()
{
	m_running = true;
	m_thread = NULL;
	m_id = nextId;
	nextId++;
}

ProcessThread::~ProcessThread()
{
	stop();
	nextId--;
}

void ProcessThread::start()
{
	if( m_thread == NULL)
	{
		m_thread = new boost::thread( boost::bind(
			&ProcessThread::body, this ) );


		// This also works but I don't know why. (Where 'this' is, should be
		// the first argument).
//		m_thread = new boost::thread( &Process::processBody, this );
	}

}

void ProcessThread::stop()
{
	if( m_thread )
	{
		// HACK: temporary solution.
		m_mutex.lock();
		m_running = false;
		m_mutex.unlock();

		// TODO: send "exit" message/event to the process.

		m_thread->join();
		delete m_thread;
		m_thread = NULL;
	}
}

void ProcessThread::restart()
{
	stop();
	start();
}

const int ProcessThread::getId() const
{
	return m_id;
}

void ProcessThread::interrupt()
{
	if( m_thread )
	{
		m_thread->interrupt();
		delete m_thread;
		m_thread = NULL;
	}
}