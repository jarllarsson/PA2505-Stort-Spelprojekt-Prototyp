/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/25/2012 												*/
/************************************************************************/

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <vector>
#include <map>
#include <string>
#include "stdafx.h"
#include "Billboard.h"
#include "PointLight.h"
#include "ShaderFactory.h"
#include "Camera.h"
#include "Mesh.h"
#include "LoadObj.h"
#include "AABB.h"
#include "Buffer.h"
#include "PVertex.h"
#include "MeshData.h"
#include "Frustum.h"
#include "MinMax.h"
#include "MeshBuffers.h"
#include "ObjData.h"
#include "ScreenAligned.h"

using namespace std;

struct BufferInfo
{
	UINT32		size;
	void*		data;
	int			numElements;
	BUFFER_TYPE	bufferType;
};


class ObjectFactory
{
private:
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	ShaderFactory*			mShaderFactory;
	Camera*					mGameCamera;
	LoadObj*				mObjParser;

	vector<pair<string,ObjData*>> m_meshList;
private:
	MinMax			FindMinMax(vector<D3DXVECTOR3> p_positions);
	AABB*			CreateABoundboxForMesh(MinMax p_minMax, D3DXCOLOR p_color);
	vector<D3DXPLANE>*		CreateFrustumPlanes(vector<D3DXVECTOR3> p_mesh);
	int				FindMeshInList(string p_mesh);
	ObjData*		CheckIfMeshHasBeenCreated(string p_mesh);
	void			PushBackNewlyCreatedMesh(string p_mesh, ObjData* p_objData);
public:
	ObjectFactory();
	~ObjectFactory();
	HRESULT		Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* gameCamera);
	Billboard*	CreateABillboard(D3DXVECTOR3 pos,float size);
	PointLight*	CreateAPointLight(Shader* shader,D3DXVECTOR3 pos,D3DXCOLOR diffuse, float range);
	Mesh*		CreateAMeshFromObj(D3DXVECTOR3 pos, string filePath, D3DXCOLOR p_debugColor);
	Frustum*	CreateAFrustum(CameraSettings p_settings, Shader* p_shader, D3DXCOLOR p_color);
	AABB*		CreateAAABB(D3DXVECTOR3 p_center, D3DXVECTOR3 p_halfDimension, D3DXCOLOR p_color);
	Buffer*		CreateMeshBuffers(BufferInfo p_buffer);
	MeshBuffers*		CreateABox();
	ScreenAligned* CreateAScreenAlignedQuad(D3DXVECTOR2 p_min, D3DXVECTOR2 p_max,ID3D11ShaderResourceView* p_samplerTarget,
											D3DXVECTOR4 p_activeSampleChannels, int negateSample=0);

};
#endif // OBJECTFACTORY_H
