// =======================================================================================
//                                      SFX
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	SFX is made to handle 3D-positional sounds only
///        
/// # SFX
/// TODO: Write details.
/// Created on: 22-11-2012 
///---------------------------------------------------------------------------------------
#ifndef SFX_H
#define SFX_H

#include "stdafx.h"
#include "SoundEffect.h"

class SFX : public SoundEffect
{
private:
	X3DAUDIO_EMITTER	m_emitter;
	X3DAUDIO_CONE		m_emitterCone;
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
public:
	SFX(X3DAUDIO_EMITTER p_emitter, X3DAUDIO_DSP_SETTINGS p_dspSettings, 
		X3DAUDIO_CONE p_cone, IXAudio2* pEngine,string pFileName,bool pLoopPlayback);
	~SFX();
	void update();
	X3DAUDIO_EMITTER		getEmitter();
	X3DAUDIO_CONE			getCone();
	X3DAUDIO_DSP_SETTINGS	getDSPSettings();
};

#endif // SFX_H