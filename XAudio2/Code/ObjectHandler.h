/************************************************************************/
/* Created by: 	Hoodad													*/
/* Date:		6/9/2012 												*/
/************************************************************************/

#ifndef OBJECTHANDLER_H__
#define OBJECTHANDLER_H__

#include "Object.h"
#include "Triangle.h"
#include "Heightmap.h"
#include "ShaderFactory.h"
#include <vector>
#include <D3D11.h>

using namespace std;

class ObjectHandler
{
private:
	vector<Object*>* objects;
	ShaderFactory* shaderFactory;
public:
	ObjectHandler();
	~ObjectHandler();
	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	HRESULT Update(float deltaTime);
	Object* GetObject(unsigned int index);
	int		GetNumObjects();

};
#endif // OBJECTHANDLER_H__
