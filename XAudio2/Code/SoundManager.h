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

using namespace std;

class SoundManager
{
private:
	HANDLE m_file;

	IXAudio2* m_soundDevice; ///< Similar to the DirectX device
	IXAudio2MasteringVoice* m_masterVoice; ///< Similar to the DirectX Context
	
	X3DAUDIO_LISTENER m_listener;
	X3DAUDIO_HANDLE m_X3DAudioInstance;

	float m_masterVolume;

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
public:
	static SoundManager* getInstance();
	static void destroy();

	///-----------------------------------------------------------------------------------
	/// This handles all the calculations 3D positional calculations
	/// \returns void
	///-----------------------------------------------------------------------------------
	void	update();

	///-----------------------------------------------------------------------------------
	/// Returns a reference to the master volume 
	/// \returns float*
	///-----------------------------------------------------------------------------------
	float*	getMasterVolume();
};

#endif // SOUNDMANAGER_H