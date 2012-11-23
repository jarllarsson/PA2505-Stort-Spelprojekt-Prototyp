/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/22/2012 												*/
/************************************************************************/

#ifndef SA_EVENT_H
#define SA_EVENT_H

#include "EventInterface.h"
#include "ScreenAligned.h"

class SA_event : public EventInterface
{
private:
	ScreenAligned* m_parent;
public:
	SA_event(ScreenAligned* p_parent){
		m_parent = p_parent;
	}
	~SA_event(){
	}
	void PerformAction()
	{
		m_parent->SwitchRenderState();
	}
};
#endif // SA_EVENT_H