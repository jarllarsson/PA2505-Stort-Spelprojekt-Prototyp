#include "Intersection.h"

Intersection::Intersection()
{

}

Intersection::~Intersection()
{

}

HRESULT Intersection::TestIntersectionTest(ObjectFactory* p_objectFactory)
{
	AABB* red = p_objectFactory->CreateAAABB(D3DXVECTOR3(0,1,0),D3DXVECTOR3(10,2.5f,10),WHITE);
	AABB* blue = p_objectFactory->CreateAAABB(D3DXVECTOR3(0,0,0),D3DXVECTOR3(20,5,20),BLACK);

	D3DXVECTOR3 point = D3DXVECTOR3(0,0,0);

	HRESULT hr = S_OK;

	if(AABBVsAABB(red,blue))
		hr = S_OK;
	else
		hr = S_FALSE;

	delete red;
	delete blue;

	return hr;
}


IntersectionTypes Intersection::PlaneVsPoint(D3DXVECTOR4 p_plane, D3DXVECTOR3 p_point)
{
	return OUTSIDE;
}


IntersectionTypes Intersection::FrustumVsAABB( Frustum* p_frustum, AABB* p_aabb )
{
	D3DXVECTOR4* planes = p_frustum->GetFrustumPlanes();
	IntersectionTypes type;
	IntersectionTypes result;

	for (int i = 0; i < 6; i++)
	{
		type = PlaneVsAABB(planes[i],p_aabb);

		if(type != OUTSIDE)
			return INTERSECTING;
	}
	
	return OUTSIDE;
}

IntersectionTypes Intersection::PlaneVsAABB(D3DXVECTOR4 p_plane, AABB* p_aabb)
{
	D3DXVECTOR3 pNormal = D3DXVECTOR3(p_plane.x,p_plane.y,p_plane.z);

	D3DXVECTOR3 center	= (p_aabb->GetMaxPos() + p_aabb->GetMinPos())*0.5f;
	D3DXVECTOR3 halfD	= (p_aabb->GetMaxPos() - p_aabb->GetMinPos())*0.5f;

	float e = (halfD.x * abs(pNormal.x)) + (halfD.y * abs(pNormal.y)) + (halfD.z*abs(pNormal.z));
	float s = D3DXVec3Dot(&center,&pNormal) + p_plane.w;

	if (s-e>0)
		return OUTSIDE;
	if (s+e<0)
		return INSIDE;

	return INTERSECTING;
}

IntersectionTypes Intersection::AABBVsAABB(AABB* p_redBox, AABB* p_blueBox)
{
	float minRed[] = {
		p_redBox->GetMinPos().x,p_redBox->GetMinPos().y,p_redBox->GetMinPos().z
	};

	float maxRed[] = {
		p_redBox->GetMaxPos().x,p_redBox->GetMaxPos().y,p_redBox->GetMaxPos().z
	};

	float minBlue[] = {
		p_blueBox->GetMinPos().x,p_blueBox->GetMinPos().y,p_blueBox->GetMinPos().z
	};

	float maxBlue[] = {
		p_blueBox->GetMaxPos().x,p_blueBox->GetMaxPos().y,p_blueBox->GetMaxPos().z
	};

	for (int i = 0; i < 3; i++)
	{
		if( minRed[i] > maxBlue[i] || minBlue[i] > maxRed[i] )
			return OUTSIDE;
	}
	return INSIDE;
}