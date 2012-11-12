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

public:
	Process();
	virtual ~Process();

	void start();
	void stop();
	void restart();
	const int getId() const;

	virtual void processBody() = 0;

};

#endif