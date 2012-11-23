/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/12/2012 												*/
/************************************************************************/

#ifndef EVENTINTERFACE_H
#define EVENTINTERFACE_H

class EventInterface
{	
public:
	EventInterface(){}
	~EventInterface(){}
	virtual void PerformAction() = 0;
};
#endif // EVENTINTERFACE_H
