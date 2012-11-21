#ifndef SFX_h__
#define SFX_h__
#include "SoundEffect.h"
#include "SoundSetup.h"
#include <D3DX10.h>

class SFX : public SoundEffect
{
private:
	D3DXVECTOR3 pos;
private:
	void calculateAndApply3DEffect();
	void calculateAndApplyVolume();
	float* calculateDolbyProLogicIIMatrixEncoding(float pPan,float pLeft,float pRight);
public:
	SFX(D3DXVECTOR3 pPos,IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels,SoundSetup pSetup);
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