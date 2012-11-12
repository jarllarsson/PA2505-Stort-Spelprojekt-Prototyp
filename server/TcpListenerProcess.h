#ifndef TCP_LISTENER_PROCESS_H
#define TCP_LISTENER_PROCESS_H

#include <ProcessThread.h>
#include <iostream>
using namespace std;

class TcpListenerProcess: public ProcessThread
{
public:
	TcpListenerProcess();
	~TcpListenerProcess();

	void processBody();

};

#endif