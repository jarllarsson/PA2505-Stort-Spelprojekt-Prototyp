#ifndef VoiceCallback_h__
#define VoiceCallback_h__
#include <XAudio2.h>
#include "SoundEffect.h"

class VoiceCallback : public IXAudio2VoiceCallback
{
private:
	SoundEffect* parent;
public:
	HANDLE bufferEndEvent;
public:
	VoiceCallback(SoundEffect* pParent): bufferEndEvent(CreateEvent (NULL,FALSE,FALSE,NULL)){
		parent = pParent;
	}
	~VoiceCallback(){
		CloseHandle(bufferEndEvent);
	}
	void __stdcall OnStreamEnd(){
		SetEvent( bufferEndEvent );
		parent->setPlaying(false);
	}
	//Unused methods are stubs
	void __stdcall OnVoiceProcessingPassEnd() { }
	void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void __stdcall OnBufferEnd(void * pBufferContext)    { }
	void __stdcall OnBufferStart(void * pBufferContext) {    }
	void __stdcall OnLoopEnd(void * pBufferContext) {    }
	void __stdcall OnVoiceError(void * pBufferContext, HRESULT Error) { }
};

#endif // VoiceCallback_h__
