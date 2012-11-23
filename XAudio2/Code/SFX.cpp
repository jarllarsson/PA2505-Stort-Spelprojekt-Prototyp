#include "SFX.h"

SFX::SFX(X3DAUDIO_EMITTER p_emitter, X3DAUDIO_DSP_SETTINGS p_dspSettings, 
		 X3DAUDIO_CONE p_cone, IXAudio2* pEngine,string pFileName,bool pLoopPlayback)
	: SoundEffect(pEngine,pFileName,pLoopPlayback)
{
	m_emitter		= p_emitter;
	m_dspSettings	= p_dspSettings;
	m_emitterCone	= p_cone;
}

SFX::~SFX()
{
}

void SFX::update()
{
}

X3DAUDIO_EMITTER SFX::getEmitter()
{
	return m_emitter;
}

X3DAUDIO_CONE SFX::getCone()
{
	return m_emitterCone;
}

X3DAUDIO_DSP_SETTINGS SFX::getDSPSettings()
{
	return m_dspSettings;
}
