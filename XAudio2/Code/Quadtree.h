/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/12/2012 												*/
/************************************************************************/

#ifndef QUADTREE_H
#define QUADTREE_H

#include "stdafx.h"
#include "Mesh.h"
#include "Object.h"
#include "AABB.h"
#include "ObjectFactory.h"
#include "Frustum.h"
#include "Intersection.h"

using namespace std;

struct AABB_quad
{
	D3DXVECTOR3 m_halfDimension;
	D3DXVECTOR3 m_vecCenter;
};

struct Node
{
	static const int	m_depth = 4;
	Mesh*				m_data;
	AABB_quad			m_boundary;
	AABB*				m_aabb;

	Node(AABB_quad p_boundary, Mesh* p_data, AABB* p_aabb)
	{
		m_data		= p_data;
		m_boundary	= p_boundary;

		m_aabb		= p_aabb;
		m_northWest = NULL;
		m_northEast = NULL;
		m_southWest = NULL;
		m_southEast = NULL;
	}

	Node*		m_northWest;
	Node*		m_northEast;
	Node*		m_southWest;
	Node*		m_southEast;
};

class Quadtree
{
private:
	Node*			m_root;
	ObjectFactory*	m_objectFactory;
	Frustum*		m_frustum;
	Intersection*	m_intersectionTesting;
private:
	void	DeleteQuadTree(Node* p_parent);
	void	RenderQuadTree(Node* p_parent, D3DXMATRIX p_viewProjection);
	void	PlaceObjectInNode(Node* p_parent, Mesh* p_object);
	bool	CanContainObject(Node* p_node, Object* p_object);
	bool	CheckCollisionWithNodeVsFrustum(Node* p_node, Frustum* p_frustum);
public:
	Quadtree(ObjectFactory* p_objectFactory, D3DXVECTOR3 p_pos, D3DXVECTOR3 p_diagonal);
	~Quadtree();
	void	GenerateQuadTree(Node* p_parent, int p_data);
	void	PlaceObjects(vector<Mesh*> p_objects);
	HRESULT Update(float dt);
	HRESULT Render(D3DXMATRIX p_viewProjection);

	//////////////////////////////////////////////////////////////////////////
	void	CheckNodesAgainstFrustum(Frustum* p_frustum);
	void	CheckNodeAgainstFrustum(Node* p_node, Frustum* p_frustum);
};
#endif // QUADTREE_H
