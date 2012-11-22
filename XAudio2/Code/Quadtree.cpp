#include "Quadtree.h"

Quadtree::Quadtree(ObjectFactory* p_objectFactory, D3DXVECTOR3 p_pos, D3DXVECTOR3 p_diagonal)
{
	m_objectFactory = p_objectFactory;

	AABB_quad rootBoundary;
	rootBoundary.m_vecCenter = p_pos;
	rootBoundary.m_halfDimension = p_diagonal;
	m_intersectionTesting = new Intersection();
	m_intersectionTesting->TestIntersectionTest(p_objectFactory);

	m_root = new Node(rootBoundary,0,m_objectFactory->CreateAAABB(rootBoundary.m_vecCenter,rootBoundary.m_halfDimension,RED));

	GenerateQuadTree(m_root,2);	
}

Quadtree::~Quadtree()
{
	DeleteQuadTree(m_root);
	SAFE_DELETE(m_intersectionTesting);
}

void Quadtree::GenerateQuadTree(Node* p_parent, int p_depth)
{
	if (p_depth > 0) //Subdived the root
	{
		p_depth -=1;

		AABB_quad bNorthWest, bNorthEast, bSouthWest, bSouthEast;

		D3DXVECTOR3 newHalfDimension = p_parent->m_boundary.m_halfDimension/2;
		newHalfDimension.y = p_parent->m_boundary.m_halfDimension.y;

		bNorthWest.m_halfDimension = bNorthEast.m_halfDimension = 
		bSouthWest.m_halfDimension = bSouthEast.m_halfDimension = newHalfDimension;

		D3DXVECTOR3 pCent = p_parent->m_boundary.m_vecCenter;

		bNorthWest.m_vecCenter = D3DXVECTOR3(pCent.x-newHalfDimension.x,pCent.y,pCent.z-newHalfDimension.z);
		bNorthEast.m_vecCenter = D3DXVECTOR3(pCent.x+newHalfDimension.x,pCent.y,pCent.z-newHalfDimension.z);
		bSouthWest.m_vecCenter = D3DXVECTOR3(pCent.x-newHalfDimension.x,pCent.y,pCent.z+newHalfDimension.z);
		bSouthEast.m_vecCenter = D3DXVECTOR3(pCent.x+newHalfDimension.x,pCent.y,pCent.z+newHalfDimension.z);

		D3DXCOLOR newColor;

		if (p_depth == 1)
			newColor = CYAN;
		else if(p_depth == 0)
			newColor = YELLOW;

		p_parent->m_northWest = new Node(bNorthWest, NULL,m_objectFactory->CreateAAABB(bNorthWest.m_vecCenter,bNorthEast.m_halfDimension,newColor));
		p_parent->m_northEast = new Node(bNorthEast, NULL,m_objectFactory->CreateAAABB(bNorthEast.m_vecCenter,bNorthEast.m_halfDimension,newColor));
		p_parent->m_southWest = new Node(bSouthWest, NULL,m_objectFactory->CreateAAABB(bSouthWest.m_vecCenter,bSouthWest.m_halfDimension,newColor));
		p_parent->m_southEast = new Node(bSouthEast, NULL,m_objectFactory->CreateAAABB(bSouthEast.m_vecCenter,bSouthEast.m_halfDimension,newColor));

		GenerateQuadTree(p_parent->m_northWest,p_depth);
		GenerateQuadTree(p_parent->m_northEast,p_depth);
		GenerateQuadTree(p_parent->m_southWest,p_depth);
		GenerateQuadTree(p_parent->m_southEast,p_depth);
	}
	else	// Contain the data in root
	{
		return;
	}
}


bool Quadtree::CanContainObject(Node* p_node, Object* p_object)
{
	D3DXVECTOR3 dist	= p_object->getPos() - p_node->m_boundary.m_vecCenter;
	if (D3DXVec3Length(&dist)<10)
		return true;
	return false;
}
void Quadtree::PlaceObjectInNode(Node* p_parent, Mesh* p_object)
{
	if(m_intersectionTesting->AABBVsAABB(p_parent->m_aabb,p_object->GetAABB()))
	{
		if(p_parent->m_northWest!=NULL)
		{
			PlaceObjectInNode(p_parent->m_northWest,p_object);
			PlaceObjectInNode(p_parent->m_northEast,p_object);
			PlaceObjectInNode(p_parent->m_southWest,p_object);
			PlaceObjectInNode(p_parent->m_southEast,p_object);
		}
		else
		{
			p_parent->m_data = p_object;
		}
	}
}
void Quadtree::PlaceObjects(vector<Mesh*> p_objects)
{
	for (unsigned int i = 0; i < p_objects.size(); i++)
	{
		PlaceObjectInNode(m_root, p_objects[i]);
	}
}

HRESULT Quadtree::Render(D3DXMATRIX p_viewProjection)
{
	HRESULT hr = S_OK;

	m_root->m_aabb->Render(p_viewProjection);
	RenderQuadTree(m_root, p_viewProjection);

	return hr;
}

void Quadtree::RenderQuadTree(Node* p_parent, D3DXMATRIX p_viewProjection)
{
	if (p_parent->m_northWest != NULL)
	{
		RenderQuadTree(p_parent->m_northWest,p_viewProjection);
		RenderQuadTree(p_parent->m_northEast,p_viewProjection);
		RenderQuadTree(p_parent->m_southWest,p_viewProjection);
		RenderQuadTree(p_parent->m_southEast,p_viewProjection);
	}
	else if (p_parent->m_data != NULL)
	{
		p_parent->m_aabb->Render(p_viewProjection);
	}
}

void Quadtree::DeleteQuadTree(Node* p_parent)
{
	if (p_parent->m_northEast != NULL)
	{
		DeleteQuadTree(p_parent->m_northWest);
		DeleteQuadTree(p_parent->m_northEast);
		DeleteQuadTree(p_parent->m_southWest);
		DeleteQuadTree(p_parent->m_southEast);

		delete p_parent->m_aabb;
		delete p_parent;
	}
	else
	{
		delete p_parent->m_aabb;
		delete p_parent;
		return;
	}
}

void Quadtree::CheckNodesAgainstFrustum( Frustum* p_frustum )
{
	CheckNodeAgainstFrustum(m_root,p_frustum);
}

void Quadtree::CheckNodeAgainstFrustum( Node* p_node, Frustum* p_frustum )
{
	if (m_intersectionTesting->FrustumVsAABB(p_frustum,p_node->m_aabb) != OUTSIDE)
	{
		if (p_node->m_northEast != NULL)
		{
			CheckNodeAgainstFrustum(p_node->m_northEast,p_frustum);
			CheckNodeAgainstFrustum(p_node->m_northWest,p_frustum);
			CheckNodeAgainstFrustum(p_node->m_southEast,p_frustum);
			CheckNodeAgainstFrustum(p_node->m_southWest,p_frustum);
		}

		if(p_node->m_data != NULL)
			p_node->m_data->SetVisible();
	}
}