#ifndef PROCESS_H
#define PROCESS_H

#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\thread.hpp>

class ProcessThread
{
protected:
	// HACK: should be handled with an exit event instead. (m_running should only
	// be in super classes.
	bool m_running;

	// HACK: mutex should be in base class only. Only events/messages should be
	// handled in super classes.
	boost::mutex m_mutex;

private:
	boost::thread* m_thread;
	int m_id;
	static int nextId;


	// TODO:
	// add a private message queue.

public:
	ProcessThread();
	virtual ~ProcessThread();

	void start();
	void stop();
	void restart();
	const int getId() const;
	void interrupt();

	// TODO:
	// public send(event). Send a message/event to the process.
	// protected receive(event). Called when message is read.

	virtual void body() = 0;

};

#endif