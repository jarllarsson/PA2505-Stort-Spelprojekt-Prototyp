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
	m_thread = new boost::thread( boost::bind(
		&Process::processBody, this ) );
}

void Process::stop()
{
	if( m_thread )
	{
		m_thread->join();
		delete m_thread;
	}
}

void Process::restart()
{
}

const int Process::getId() const
{
	return m_id;
}