#include "SoundManager.h"
#include <XAPO.h>
#include <XAPOFX.h>
#include <XAPOBase.h>
#include <X3DAudio.h>

SoundManager* SoundManager::sInstance = 0;

SoundManager* SoundManager::getInstace()
{
	if (sInstance==0)
		sInstance = new SoundManager();
	return sInstance;
}

void SoundManager::destroy()
{
	delete sInstance;
}

SoundManager::SoundManager(){}

SoundManager::~SoundManager()
{
	for (unsigned int i = 0; i < sounds.size();i++)
	{
		delete sounds[i];
	}

	audio->StopEngine();
	audio->Release();
}


void SoundManager::init()
{
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	audio = NULL;
	HRESULT hr;
	hr = XAudio2Create( &audio,XAUDIO2_DEBUG_ENGINE,XAUDIO2_DEFAULT_PROCESSOR );

	XAUDIO2_DEVICE_DETAILS details;
	audio->GetDeviceDetails(0,&details);
	int channels = details.OutputFormat.Format.nChannels;

	if (channels == 2)
		setup = STEREO;
	else if (channels == 6)
		setup = SURROUND51;
	masterVoice = NULL;
	hr = audio->CreateMasteringVoice(&masterVoice);
	
	//setup = SURROUND51;
	//hr = audio->CreateMasteringVoice(&masterVoice,6,44100,0,0,0);

	backgroundMusic = new SoundEffect(masterVoice,audio,L"Sound/AAAHHH.wav",true,2,setup);
	D3DXVECTOR3 gunShotPos = D3DXVECTOR3(-10,0,0);
	gunShot = new SFX(gunShotPos,masterVoice,audio,L"Sound/500Calibur.wav",false,1,setup);

	D3DXVECTOR3 surMusicPos = D3DXVECTOR3(10,10,0);
	positionalMusic = new SFX(surMusicPos,masterVoice,audio,L"Sound/AAAHHH_mono.wav",true,1,setup);
	sounds.push_back(backgroundMusic);
	sounds.push_back(gunShot);
	sounds.push_back(positionalMusic);
	
	IID apoId;
	hr = CoCreateGuid((GUID*)(&apoId));
	XAPO_REGISTRATION_PROPERTIES props;

//	lowPassFilter = new LowPass(&props);
}
void SoundManager::update()
{
	for (unsigned int i = 0; i < sounds.size();i++)
	{
		sounds[i]->update();
	} 
}

void SoundManager::startStopMusic()
{
	if(backgroundMusic->isPlaying())
		backgroundMusic->stop();
	else
		backgroundMusic->play();
}

void SoundManager::playGunShot()
{
	gunShot->play();
}

void SoundManager::playPositionalMusic()
{
	if (positionalMusic->isPlaying())
		positionalMusic->stop();
	else
		positionalMusic->play();
}

SFX* SoundManager::createSoundEffect(wchar_t* pFileName,D3DXVECTOR3 pPos,bool pLoopPlayBack)
{
	SFX* newSound = new SFX(pPos,masterVoice,audio,pFileName,pLoopPlayBack,1,setup);

	return newSound;
}