/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		10/10/2012 												*/
/************************************************************************/

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Buffer.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"
#include <vector>

class ObjectFactory;

using namespace std;

enum FrustumPlanes
{
	NEAR_PLANE, FAR_PLANE, TOP_PLANE, BOTTOM_PLANE, RIGHT_PLANE, LEFT_PLANE
};

class Frustum : public Object
{
private:
	Buffer					*m_vertexBuffer;
	Buffer					*m_normalsBuffer;
	Shader					*m_shader;
	ID3D11DeviceContext*	m_deviceContext;
	D3DXCOLOR				m_color;
	vector<D3DXPLANE>		*m_frustumPlanes;
	vector<D3DXVECTOR3>		m_frustumCorners;
	D3DXMATRIX				m_viewProjection;
	Camera*					m_camera;
	float					m_rotation;

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR4*			m_newFrustumPlanes;
private:
	vector<D3DXPLANE>*		CreateFrustumPlanes(vector<D3DXVECTOR3> p_mesh);
	D3DXPLANE				CreateAPlane(D3DXVECTOR3 p_p0, D3DXVECTOR3 p_p1, D3DXVECTOR3 p_p2);
	void					PutNormalInWorldSpace(int p_p0, int p_p1, int p_p2, int p_p3, int p_normal, vector<D3DXVECTOR3>* p_positions);
public:
	Frustum(ObjectFactory* p_factory,ID3D11DeviceContext* p_deviceContext, vector<D3DXVECTOR3> p_frustumPoints, 
			Buffer* p_vertexBuffer, Shader* p_shader, Camera* p_camera, D3DXCOLOR p_color);
	~Frustum();
	HRESULT Update(float deltaTime);
	HRESULT Render(D3DXMATRIX viewProjection);

	void	GenerateFrustumNormals(ID3D11Device* p_device);
	D3DXVECTOR4* GetFrustumPlanes();
	Camera* GetFrustumCamera();
	float*	GetRotation();
};
#endif // FRUSTUM_H
