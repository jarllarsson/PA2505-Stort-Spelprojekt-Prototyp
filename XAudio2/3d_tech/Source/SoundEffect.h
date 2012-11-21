#ifndef SoundEffect_h__
#define SoundEffect_h__
#include <XAudio2.h>
#include <D3DX10.h>
#include "SoundSetup.h"

class VoiceCallback;

class SoundEffect
{
protected:
	HANDLE file;
	wchar_t* fileName;
	IXAudio2SourceVoice* source;
	IXAudio2MasteringVoice* masterVoice;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	int numChannels;
	bool playing;
	SoundSetup setup;
	VoiceCallback* callback;
private:
	void init(bool pLoopPlayback);
public:
	SoundEffect(IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels,SoundSetup pSetup);
	~SoundEffect();
	HRESULT play();
	HRESULT stop();
	virtual void update();
	IXAudio2SourceVoice* getSource(){
		return source;
	}
	HRESULT findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT initFile();

	bool isPlaying();

	SoundSetup getSetup(){
		return setup;
	}
	void setPlaying(bool pPlaying){
		playing = pPlaying;
	}
};
#endif // SoundEffect_h__
