/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/22/2012 												*/
/************************************************************************/

#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include "stdafx.h"

struct CameraSettings
{
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float pitch;
	float heading;
	D3DXVECTOR3 pos;
};
#endif // CAMERASETTINGS_H