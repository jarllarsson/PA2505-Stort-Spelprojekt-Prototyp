#include "SFX.h"

SFX::SFX(X3DAUDIO_EMITTER p_emitter,X3DAUDIO_HANDLE* p_X3DAudioInstance,IXAudio2* pEngine, 
	string pFileName,bool pLoopPlayback)
	: SoundEffect(pEngine,pFileName,pLoopPlayback)
{
	m_emitter = p_emitter;
	m_X3DAudioInstance = p_X3DAudioInstance;
}

SFX::~SFX()
{
}

void SFX::update()
{
}
