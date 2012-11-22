#ifndef SoundEffect_h__
#define SoundEffect_h__

#include <XAudio2.h>
#include <string>
#include "SoundEnums.h"

using namespace std;

class SoundEffect
{
protected:
	HANDLE file;
	IXAudio2SourceVoice* source;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	bool playing;
private:
	void init(bool pLoopPlayback, string p_filePath);
public:
	SoundEffect(IXAudio2* pEngine, string pFileName,bool pLoopPlayback);
	~SoundEffect();
	HRESULT play();
	HRESULT stop();
	virtual void update();
	IXAudio2SourceVoice* getSource(){
		return source;
	}
	HRESULT findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT initFile(string p_filePath);

	bool isPlaying();

	void setPlaying(bool pPlaying){
		playing = pPlaying;
	}
};
#endif // SoundEffect_h__
