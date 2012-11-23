// =======================================================================================
//                                      SoundManager
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Takes care of the sounds, deleting and initing
///        
/// # SoundManager
/// TODO: Write details
/// Created on: 22-11-2012 
///---------------------------------------------------------------------------------------
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3DAudio.h>
#include <string>
#include "SoundEnums.h"
#include "SFX.h"
#include "Camera.h"

using namespace std;

// Specify sound cone to add directionality to listener for artistic effect:
// Emitters behind the listener are defined here to be more attenuated,
// have a lower LPF cutoff frequency,
// yet have a slightly higher reverb send level.
static const X3DAUDIO_CONE Listener_DirectionalCone = 
{ X3DAUDIO_PI*5.0f/6.0f, X3DAUDIO_PI*11.0f/6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

// Specify LFE level distance curve such that it rolls off much sooner than
// all non-LFE channels, making use of the subwoofer more dramatic.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = 
{ 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve          = 
{ (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

// Specify reverb send level distance curve such that reverb send increases
// slightly with distance before rolling off to silence.
// With the direct channels being increasingly attenuated with distance,
// this has the effect of increasing the reverb-to-direct sound ratio,
// reinforcing the perception of distance.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = 
{ 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve          = 
{ (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

class SoundManager
{
private:
	HANDLE m_file;

	IXAudio2* m_soundDevice; ///< Similar to the DirectX device
	IXAudio2MasteringVoice* m_masterVoice; ///< Similar to the DirectX Context
	
	X3DAUDIO_LISTENER	m_listener;
	X3DAUDIO_HANDLE		m_X3DAudioInstance;
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
	FLOAT32*			m_matrixCoefficients;
	FLOAT32*			m_emitterAzimuths;

	float m_masterVolume;
	float m_left;
	float m_right;
	DWORD m_destChannels;

	SFX* m_music;
	static SoundManager* s_instance;
private:
	SoundManager();
	virtual ~SoundManager();

	///-----------------------------------------------------------------------------------
	/// Is called when s_instance == NULL and it initialize the SoundManager
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT	init();
	///-----------------------------------------------------------------------------------
	/// Initialize hard coded values for the listener
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT initListener();

	///-----------------------------------------------------------------------------------
	/// Initialize hard coded values for the emitter
	/// \param X3DAUDIO_EMITTER * p_emitter
	/// \param X3DAUDIO_CONE * p_cone
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT initEmitter(X3DAUDIO_EMITTER* p_emitter , X3DAUDIO_CONE* p_cone);

	///-----------------------------------------------------------------------------------
	/// Initialize hard coded values for the a dummy sound
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT initPositionalSound();

	///-----------------------------------------------------------------------------------
	/// Called every frame to update the listener
	/// \param Camera * p_gameCamera
	/// \returns void
	///-----------------------------------------------------------------------------------
	void updateListener(Camera* p_gameCamera);

public:
	static SoundManager* getInstance();
	static void destroy();

	///-----------------------------------------------------------------------------------
	/// This handles all the calculations 3D positional calculations
	/// \returns void
	///-----------------------------------------------------------------------------------
	void	update(Camera* p_gameCamera);

	///-----------------------------------------------------------------------------------
	/// Returns a reference to the master volume 
	/// \returns float*
	///-----------------------------------------------------------------------------------
	float*	getMasterVolume();

	float*	getLeftVolume();
	float*	getRightVolume();
};

#endif // SOUNDMANAGER_H