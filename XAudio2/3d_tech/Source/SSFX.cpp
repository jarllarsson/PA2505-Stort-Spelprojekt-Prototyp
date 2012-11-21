#include "SSFX.h"

SSFX::SSFX(D3DXVECTOR3 pPos,IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels) : SFX(pos,pMasterVoice,pEngine,pFileName,pLoopPlayback,pNumOfChannels)
{

}

SSFX::~SSFX()
{

}

void SSFX::calculateOutputMatrix(float left, float right)
{
	float* outMatrix = new float[numChannels*6]; //1*5+1 = 5.1
	/*
	outMatrix[0] = left;	//Left
	outMatrix[1] = 0.0f;	//Left,right
	outMatrix[2] = 0.0f;	//Right,left
	outMatrix[3] = right;	//Right
	outMatrix[4] = 0.0f;	//Front Center,left
	outMatrix[5] = 0.0f;	//Front Center,right
	outMatrix[6] = 0.0f;	//LEF ?!
	outMatrix[7] = 0.0f;	//LEF ?!
	outMatrix[8] = left*0.8f;	//Rear left,left
	outMatrix[9] = 0.0f;	//Rear left,right
	outMatrix[10] = 0.0f;	//Rear right, left
	outMatrix[11] = right*0.8f;	//Rear right, right
	*/

	outMatrix[0] = left;
	outMatrix[1] = right;
	outMatrix[2] = 0.0f;
	outMatrix[3] = 0.0f;
	outMatrix[4] = left*0.8f;
	outMatrix[5] = right*0.8f;
	
	source->SetOutputMatrix(masterVoice,numChannels,6,outMatrix);
}