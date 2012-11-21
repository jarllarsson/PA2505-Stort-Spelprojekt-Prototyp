#ifndef SSFX_h__
#define SSFX_h__
#include "SFX.h"

class SSFX : public SFX
{
private:
	D3DXVECTOR3 pos;
private:
	void calculateAndApply3D();
	float calculateDistanceFromCamera();
public:
	SSFX(D3DXVECTOR3 pPos,IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels);
	~SSFX();
	void calculateOutputMatrix(float left, float right);
};

#endif // SSFX_h__