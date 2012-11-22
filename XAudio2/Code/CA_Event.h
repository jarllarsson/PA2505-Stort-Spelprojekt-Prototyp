/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/22/2012 												*/
/************************************************************************/

#ifndef CA_EVENT_H
#define CA_EVENT_H

#include "EventInterface.h"
#include "Camera.h"

class CA_Event : public EventInterface
{
private:
	Camera* m_parent;
public:
	CA_Event(Camera* p_parent)
	{
		m_parent = p_parent;
	}
	~CA_Event(){

	}
	void PerformAction()
	{
		m_parent->ChangeFPSCameraState();
	}
};
#endif // CA_EVENT_H