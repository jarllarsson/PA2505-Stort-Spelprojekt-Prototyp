#include "Frustum.h"
#include "ObjectFactory.h"

Frustum::Frustum(ObjectFactory* p_factory,ID3D11DeviceContext* p_deviceContext, 
	vector<D3DXVECTOR3> p_frustumPoints, Buffer* p_vertexBuffer, Shader* p_shader, 
	Camera* p_camera, D3DXCOLOR p_color)
	: Object (p_camera->getPos())
{
	m_vertexBuffer		= p_vertexBuffer;
	m_shader			= p_shader;
	m_deviceContext		= p_deviceContext;
	m_color				= p_color;
	m_frustumCorners	= p_frustumPoints;
	m_camera			= p_camera;
	m_rotation			= 0;

	m_frustumPlanes = CreateFrustumPlanes(p_frustumPoints);

	vector<D3DXVECTOR3> positions;
	PutNormalInWorldSpace(0,1,2,3,NEAR_PLANE,&positions);
	PutNormalInWorldSpace(5,4,7,6,FAR_PLANE,&positions);
	PutNormalInWorldSpace(4,5,1,0,TOP_PLANE,&positions);
	PutNormalInWorldSpace(3,2,6,7,BOTTOM_PLANE,&positions);
	PutNormalInWorldSpace(1,5,6,2,RIGHT_PLANE,&positions);
	PutNormalInWorldSpace(4,0,3,7,LEFT_PLANE,&positions);

	BufferInfo vertex;
	vertex.size = sizeof(D3DXVECTOR3);
	vertex.numElements = positions.size();
	vertex.data = &positions[0];
	vertex.bufferType = VERTEX_BUFFER;

	m_normalsBuffer = p_factory->CreateMeshBuffers(vertex);
	m_newFrustumPlanes = new D3DXVECTOR4[6];
}

Frustum::~Frustum()
{
	SAFE_DELETE(m_normalsBuffer);
	SAFE_DELETE(m_vertexBuffer);
	SAFE_DELETE(m_frustumPlanes);
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_newFrustumPlanes);
}

HRESULT Frustum::Update(float deltaTime)
{
	HRESULT hr = S_OK;
	
	m_camera->adjustHeadinPitch(m_rotation*deltaTime,0);
	m_camera->Update(deltaTime);

	return hr;
}

HRESULT Frustum::Render(D3DXMATRIX viewProjection)
{
	HRESULT hr = S_OK;
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	D3DXMATRIX wvp;
	wvp = m_camera->GetWorldMatrix() * viewProjection;

	//Draw the Frustum lines
	m_vertexBuffer->Apply();
	m_shader->SetMatrix("gWVP", wvp);
	m_shader->SetFloat4("color", D3DXVECTOR4(m_color.r,m_color.g,m_color.b,m_color.a));
	m_shader->Apply(0);
	m_deviceContext->Draw(m_vertexBuffer->GetElementCount(),0);

	//DRAW THE PLANES NORMALS
	m_normalsBuffer->Apply();
	m_shader->SetMatrix("gWVP", wvp);
	m_shader->SetFloat4("color", D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f));
	m_shader->Apply(0);
	m_deviceContext->Draw(m_normalsBuffer->GetElementCount(),0);
	return hr;
}

D3DXVECTOR4* Frustum::GetFrustumPlanes()
{
	D3DXMATRIX& m = m_camera->getViewProjMatrix();
	// Left plane
	m_newFrustumPlanes[0] = D3DXVECTOR4(
		-(m._14 + m._11),
		-(m._24 + m._21),
		-(m._34 + m._31),
		-(m._44 + m._41)
		);
	// Right plane
	m_newFrustumPlanes[1] = D3DXVECTOR4(
		-(m._14 - m._11),
		-(m._24 - m._21),
		-(m._34 - m._31),
		-(m._44 - m._41)
		);
	// Top plane
	m_newFrustumPlanes[2] = D3DXVECTOR4(
		-(m._14 - m._12),
		-(m._24 - m._22),
		-(m._34 - m._32),
		-(m._44 - m._42)
		);
	// Bottom plane
	m_newFrustumPlanes[3] = D3DXVECTOR4(
		-(m._14 + m._12),
		-(m._24 + m._22),
		-(m._34 + m._32),
		-(m._44 + m._42)
		);
	// Near plane
	m_newFrustumPlanes[4] = D3DXVECTOR4(
		-(m._14 + m._13),
		-(m._24 + m._23),
		-(m._34 + m._33),
		-(m._44 + m._43)
		);
	// Far plane
	m_newFrustumPlanes[5] = D3DXVECTOR4(
		-(m._14 - m._13),
		-(m._24 - m._23),
		-(m._34 - m._33),
		-(m._44 - m._43)
		);

	return m_newFrustumPlanes;
}

vector<D3DXPLANE>* Frustum::CreateFrustumPlanes(vector<D3DXVECTOR3> p_mesh)
{
	vector<D3DXPLANE>* planes = new vector<D3DXPLANE>;
	planes->push_back(CreateAPlane(p_mesh[0],p_mesh[1],p_mesh[2]));	//NEAR
	planes->push_back(CreateAPlane(p_mesh[5],p_mesh[4],p_mesh[7]));	//FAR

	planes->push_back(CreateAPlane(p_mesh[4],p_mesh[5],p_mesh[1]));	//TOP
	planes->push_back(CreateAPlane(p_mesh[3],p_mesh[2],p_mesh[6]));	//BOTTOM

	planes->push_back(CreateAPlane(p_mesh[1],p_mesh[5],p_mesh[6]));	//RIGHT
	planes->push_back(CreateAPlane(p_mesh[4],p_mesh[0],p_mesh[3]));	//LEFT

	return planes;
}

D3DXPLANE Frustum::CreateAPlane(D3DXVECTOR3 p_p0, D3DXVECTOR3 p_p1, D3DXVECTOR3 p_p2)
{
	D3DXVECTOR3 u = p_p1 - p_p0;
	D3DXVECTOR3 v = p_p2 - p_p0;

	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal,&u,&v);

	D3DXPLANE newPlane;
	newPlane.a	= normal.x;
	newPlane.b	= normal.y;
	newPlane.c	= normal.z;

	normal		= normal*-1.0f;
	float d		= D3DXVec3Dot(&normal,&p_p0);
	newPlane.d	= d;

	D3DXPlaneNormalize(&newPlane,&newPlane);

	return newPlane;
}

void Frustum::PutNormalInWorldSpace( int p_p0, int p_p1, int p_p2, int p_p3, int p_normal, vector<D3DXVECTOR3>* p_positions)
{
	D3DXVECTOR3 p0 = m_frustumCorners[p_p0];
	D3DXVECTOR3 p1 = m_frustumCorners[p_p1];
	D3DXVECTOR3 p2 = m_frustumCorners[p_p2];
	D3DXVECTOR3 p3 = m_frustumCorners[p_p3];
	D3DXVECTOR3 normal = m_frustumPlanes->at(p_normal);

	D3DXVECTOR3 u = (p1-p0)/2;
	D3DXVECTOR3 v = (p2-p3)/2;
	D3DXVECTOR3 pHalfU = u + p0;
	D3DXVECTOR3 pHalfV = v + p3;
	
	D3DXVECTOR3 vCenter = (pHalfU - pHalfV)/2;
	D3DXVECTOR3 center = vCenter + pHalfV;
	D3DXVECTOR3 offsetedAlongNormal = center+(normal*3);

	p_positions->push_back(center);
	p_positions->push_back(offsetedAlongNormal);
}

Camera* Frustum::GetFrustumCamera()
{
	return m_camera;
}
float* Frustum::GetRotation()
{
	return &m_rotation;
}