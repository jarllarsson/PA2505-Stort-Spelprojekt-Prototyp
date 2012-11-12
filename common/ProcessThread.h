#ifndef PROCESS_H
#define PROCESS_H

#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\thread.hpp>

class ProcessThread
{
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

	// TODO:
	// public send(event). Send a message/event to the process.
	// protected receive(event). Called when message is read.

	virtual void body() = 0;

};

#endif