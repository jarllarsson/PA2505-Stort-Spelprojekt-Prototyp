#ifndef TCP_LISTENER_PROCESS_H
#define TCP_LISTENER_PROCESS_H

#include "Process.h"
#include <iostream>
using namespace std;

class TcpListenerProcess: public Process
{
public:
	TcpListenerProcess();
	~TcpListenerProcess();

	void processBody();

};

#endif