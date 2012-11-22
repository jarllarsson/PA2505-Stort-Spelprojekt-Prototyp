#ifndef SFX_h__
#define SFX_h__
#include "stdafx.h"
#include "SoundEffect.h"

class SFX : public SoundEffect
{
private:
	D3DXVECTOR3 pos;
private:
	void calculateAndApply3DEffect();
	void calculateAndApplyVolume();
	float* calculateDolbyProLogicIIMatrixEncoding(float pPan,float pLeft,float pRight);
public:
	SFX(D3DXVECTOR3 pPos,IXAudio2* pEngine,string pFileName,bool pLoopPlayback);
	~SFX();
	void update();
	
	virtual void calculateOutputMatrix(float pan);

	void setPos(D3DXVECTOR3 pPos){
		pos = pPos;
	}
	D3DXVECTOR3 getPos(){
		return pos;
	}
};

#endif // 3DSFX_h__