/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/25/2012 												*/
/************************************************************************/

#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "stdafx.h"
#include "AABB.h"
#include "Frustum.h"
#include "ObjectFactory.h"

class Intersection
{

public:
	Intersection();
	~Intersection();
	IntersectionTypes PlaneVsPoint(D3DXVECTOR4 p_plane, D3DXVECTOR3 p_point);
	IntersectionTypes FrustumVsAABB(Frustum* p_frustum, AABB* p_aabb);
	IntersectionTypes PlaneVsAABB(D3DXVECTOR4 p_plane, AABB* p_aabb);
	IntersectionTypes AABBVsAABB(AABB* p_redBox, AABB* p_blueBox);
	HRESULT TestIntersectionTest(ObjectFactory* p_objectFactory);
};
#endif // INTERSECTION_H