#include "SoundManager.h"

SoundManager* SoundManager::s_instance = NULL;

SoundManager::SoundManager()
{
	m_soundDevice = NULL;
	m_masterVoice = NULL;

	init();
}

SoundManager::~SoundManager()
{

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

}

void SoundManager::init()
{
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	if( FAILED( hr = XAudio2Create(m_soundDevice)) )
}
