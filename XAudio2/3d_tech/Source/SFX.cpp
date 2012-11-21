#include "SFX.h"
#include "Camera.h"
#include "Globals.h"

SFX::SFX(D3DXVECTOR3 pPos,IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels,SoundSetup pSetup) : SoundEffect(pMasterVoice,pEngine,pFileName,pLoopPlayback,pNumOfChannels,pSetup)
{
	pos = pPos;
}

SFX::~SFX()
{
}

void SFX::update()
{
	calculateAndApply3DEffect();
}

void SFX::calculateAndApply3DEffect()
{
	D3DXVECTOR3 view = Camera::getInstance()->getForward();
	D3DXVECTOR3 up = Camera::getInstance()->getUp();
	D3DXVECTOR3 camPos = Camera::getInstance()->getPos();

	D3DXVECTOR3 sl = pos-camPos;
	D3DXVec3Normalize(&sl,&sl);

	D3DXVECTOR3 ab;
	D3DXVec3Cross(&ab,&view,&up);
	D3DXVec3Normalize(&ab,&ab);

	float pan = D3DXVec3Dot(&ab,&sl);

	calculateOutputMatrix(pan);
	calculateAndApplyVolume();
}

void SFX::calculateAndApplyVolume()
{
	D3DXVECTOR3 camPos = Camera::getInstance()->getPos();
	D3DXVECTOR3 distance = pos-camPos;

	//Notice the division!
	float length = D3DXVec3Length(&distance)/2;

	if (length<1000)
	{
		length = length*-1;

		float raisedAmount = (length/20);
		float amp = pow(10,raisedAmount);

		source->SetVolume(amp);
	}
	else
		source->SetVolume(0);
}
void SFX::calculateOutputMatrix(float pPan)
{
	float left = SQRTTWO*(cos(pPan)+sin(pPan));
	float right = SQRTTWO*(cos(pPan)-sin(pPan));

	float* outMatrix;
	int destChannels;
	
	switch (setup)
	{
	case STEREO:
		destChannels = 2;
		outMatrix = new float[numChannels*destChannels];
		outMatrix[0] = left;
		outMatrix[1] = right;
		break;
	case SURROUND51:
		float* channels = calculateDolbyProLogicIIMatrixEncoding(pPan,left,right);
		destChannels = 6;
		outMatrix = new float[numChannels*destChannels];
		outMatrix[0] = left;		//FrontLeft
		outMatrix[1] = right;		//FrontRight
		outMatrix[2] = channels[0];	//FrontCenter
		outMatrix[3] = 0.0f;		//LFE (Low frequency channel aka subwofer)
		outMatrix[4] = channels[1];	//SideLeft
		outMatrix[5] = channels[2];	//SideRight
		break;
	}

	source->SetOutputMatrix(masterVoice,numChannels,destChannels,outMatrix);
}

float* SFX::calculateDolbyProLogicIIMatrixEncoding(float pPan,float pLeft,float pRight)
{
	float* output = new float[3];

	float j = SQRTTWO*(cos(pPan+HALFPI)+sin(pPan+HALFPI));
	float k = SQRTTWO*(cos(pPan-HALFPI)-sin(pPan-HALFPI));

	output[0] = pLeft*sqrt(1.0f/2.0f)+pRight*sqrt(1.0f/2.0f);
	output[1] = j*sqrt(2.0f/3.0f)+k*sqrt(1.0f/3.0f);
	output[2] = j*sqrt(1.0f/3.0f)+k*sqrt(2.0f/3.0f);

	return output;
}
