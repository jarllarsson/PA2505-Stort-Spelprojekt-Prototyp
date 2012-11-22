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

using namespace std;

class SoundManager
{
private:
	HANDLE m_file;

	IXAudio2* m_soundDevice;
	IXAudio2SourceVoice* m_sourceVoice;
	IXAudio2MasteringVoice* m_masterVoice;
	XAUDIO2_BUFFER m_buffer;
	WAVEFORMATEXTENSIBLE m_wfx;

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
	void update();

	void PlaySound( string p_filePath);
	HRESULT findChunk(	HANDLE p_file, DWORD p_fourcc, DWORD& p_dwChunkSize, 
						DWORD& p_dwChunkDataPosition);
	HRESULT readChunkData(	HANDLE p_file, void* p_buffer, DWORD p_bufferSize, 
							DWORD p_bufferOffset);
	HRESULT initFile(string p_filePath);
};

#endif // SOUNDMANAGER_H