#ifndef Box_h__
#define Box_h__
#include <d3d10.h>
#include <D3DX10.h>
#include "SVertex.h"
#include "Object.h"
#include "MovementComp.h"
#include "SoundComp.h"
#include "SFX.h"

class Box : public Object
{
private:
	ID3D10Device* device;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	DWORD numVertices;
	DWORD numIndices;

	ID3D10Effect* effect;
	ID3D10EffectTechnique* effectTechnique;
	ID3D10EffectPass* effectPass;
	ID3D10InputLayout* inputLayout;
	ID3D10EffectMatrixVariable* worldMatrix;    // the pointer to the effect variable interface

	MovementComp* movComp;
	SoundComp* soundComp;
protected:
public:
	Box(ID3D10Device* pDevice,D3DXVECTOR3 pPos,D3DXCOLOR pColor, float pScale);
	~Box();
	void draw();
	void update(float deltaTime);
	void initPipeline();
	void resolve(SFX* sound,char key,bool enableMovement);
};
#endif // Box_h__
