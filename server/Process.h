#ifndef PROCESS_H
#define PROCESS_H

#include <boost\thread.hpp>
#include <boost\bind.hpp>

class Process
{
private:
	boost::thread* m_thread;
	int m_id;
	static int nextId;

	// TODO:
	// add a private message queue.

public:
	Process();
	virtual ~Process();

	void start();
	void stop();
	void restart();
	const int getId() const;

	// TODO:
	// public send(event). Send a message/event to the process.
	// protected receive(event). Called when message is read.

	virtual void processBody() = 0;

};

#endif