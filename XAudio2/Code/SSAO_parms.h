/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		24/10/2012 												*/
/************************************************************************/

#ifndef SSAO_PARMS_H
#define SSAO_PARMS_H

#include "stdafx.h"

struct SSAO_parms
{
	D3DXVECTOR2 gScreenSize;
	float gRandomTexSize;
	float gScale;
	float gBias;
	float gIntensity;
	float gSampleRadius;
};

#endif