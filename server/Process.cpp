#include "Process.h"

int Process::nextId = 0;

Process::Process()
{
	m_thread = NULL;
	m_id = nextId;
	nextId++;
}

Process::~Process()
{
	stop();
	nextId--;
}

void Process::start()
{
	if( m_thread == NULL)
	{
		m_thread = new boost::thread( boost::bind(
			&Process::processBody, this ) );


		// This also works but I don't know why. (Where 'this' is, should be
		// the first argument).
//		m_thread = new boost::thread( &Process::processBody, this );
	}

}

void Process::stop()
{
	if( m_thread )
	{
		// TODO: send "exit" message/event to the process.

		m_thread->join();
		delete m_thread;
		m_thread = NULL;
	}
}

void Process::restart()
{
	stop();
	start();
}

const int Process::getId() const
{
	return m_id;
}