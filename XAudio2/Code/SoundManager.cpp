#include "SoundManager.h"

SoundManager* SoundManager::s_instance = NULL;

SoundManager::SoundManager()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_music			= NULL;
	m_masterVolume	= 0;


	/************************************************************************/
	/* Don't looks much to the world but here is where all the magic is done*/
	/************************************************************************/
	init();

	m_masterVoice->GetVolume(&m_masterVolume);
}

SoundManager::~SoundManager()
{
	m_soundDevice->StopEngine();
	m_soundDevice->Release();
}

SoundManager* SoundManager::getInstance()
{
	if(s_instance==NULL)
		s_instance = new SoundManager();
	return s_instance;
}

void SoundManager::destroy()
{
	if(s_instance!=NULL)
		delete s_instance;
}

void SoundManager::update()
{
	m_masterVoice->SetVolume(m_masterVolume,0);
}

HRESULT SoundManager::init()
{
	HRESULT hr = S_OK;
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// Send in the wanted flags when the sound device is created.
	if( FAILED( hr = XAudio2Create(&m_soundDevice, flags)) )
	{
		return hr;
	}

	if( FAILED( hr = m_soundDevice->CreateMasteringVoice(&m_masterVoice)))
	{
		return hr;
	}

	XAUDIO2_DEVICE_DETAILS details;
	if( FAILED( hr = m_soundDevice->GetDeviceDetails(0, &details)))
	{
		return hr;
	}

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, 
		m_X3DAudioInstance);

	X3DAUDIO_EMITTER emitter;
	emitter.ChannelCount = details.OutputFormat.Format.nChannels;
	emitter.Position = D3DXVECTOR3(0,0,0);

	m_music = new SFX(emitter,m_soundDevice,"../Sound/Techno_1.wav",true);
	m_music->play();

	return hr;
}

float* SoundManager::getMasterVolume()
{
	return &m_masterVolume;
}
