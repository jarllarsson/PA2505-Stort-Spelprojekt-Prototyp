/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		25/6/2012 												*/
/************************************************************************/

#ifndef LIGHTINFO_H__
#define LIGHTINFO_H__

struct LightInfo 
{
	LightInfo()
	{
		ZeroMemory(this, sizeof(LightInfo));
	}

	D3DXVECTOR3		pos;
	float			pad;
	D3DXCOLOR		diffuse;
	float			range;
	float			pad1,
					pad2,
					pad3;
};

#endif // LIGHTINFO_H__
