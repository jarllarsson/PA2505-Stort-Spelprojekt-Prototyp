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
#include <strsafe.h> // Used to get the FAILED

class SoundManager
{
private:
	IXAudio2* m_soundDevice;
	IXAudio2MasteringVoice* m_masterVoice;

	static SoundManager* s_instance;
private:
	SoundManager();
	virtual ~SoundManager();

	///-----------------------------------------------------------------------------------
	/// Is called when s_instance == NULL and it initialize the SoundManager
	/// \returns void
	///-----------------------------------------------------------------------------------
	void	init();
public:
	static SoundManager* getInstance();
	static void destroy();

	///-----------------------------------------------------------------------------------
	/// This handles all the calculations 3D positional calculations
	/// \returns void
	///-----------------------------------------------------------------------------------
	void update();
};

#endif // SOUNDMANAGER_H