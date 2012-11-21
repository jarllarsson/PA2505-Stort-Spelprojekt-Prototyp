#ifndef SoundManager_h__
#define SoundManager_h__
#include <D3DX10.h>
#include <XAudio2.h>
#include <XAudio2fx.h>
#include "SoundEffect.h"
#include "SFX.h"
#include "SoundSetup.h"
#include <vector>
#include "LowPass.h"

class SoundManager
{
private:
	IXAudio2* audio;
	IXAudio2MasteringVoice* masterVoice;

	SoundEffect* backgroundMusic;
	SFX* gunShot;
	SFX* positionalMusic;

	LowPass* lowPassFilter;

	std::vector<SoundEffect*> sounds;
	SoundSetup setup;
private:
	SoundManager();
	~SoundManager();
	static SoundManager* sInstance;
protected:
public:
	static SoundManager* getInstace();
	static void destroy();

	void update();
	void init();
	void startStopMusic();
	void playGunShot();
	void playPositionalMusic();
	SFX* createSoundEffect(wchar_t* pFileName,D3DXVECTOR3 pPos,bool pLoopPlayBack);
};
#endif // SoundManager_h__
