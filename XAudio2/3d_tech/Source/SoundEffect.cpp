#include "SoundEffect.h"
#include "Globals.h"
#include "Camera.h"
#include <math.h>
#include "VoiceCallback.h"

SoundEffect::SoundEffect(IXAudio2MasteringVoice* pMasterVoice,IXAudio2* pEngine,wchar_t* pFileName,bool pLoopPlayback,int pNumOfChannels,SoundSetup pSetup) 
{
	fileName = pFileName;
	masterVoice = pMasterVoice;
	numChannels = pNumOfChannels;
	playing = false;
	setup = pSetup;

	init(pLoopPlayback);

	HRESULT hr;
	callback = new VoiceCallback(this);
	hr = pEngine->CreateSourceVoice(&source, (WAVEFORMATEX*)&wfx,0,XAUDIO2_DEFAULT_FREQ_RATIO,callback,NULL,NULL);
}
SoundEffect::~SoundEffect()
{
}
void SoundEffect::init(bool pLoopPlayback)
{	
	HRESULT hr;
	hr = initFile();

	DWORD chunkSize, chunkPosition, fileType;
	findChunk(file,fourccRIFF,chunkSize,chunkPosition);

	readChunkData(file,&fileType,sizeof(DWORD), chunkPosition);

	if (fileType != fourccWAVE)
		hr = S_FALSE;

	/*************************************************************************/
	/* Locate the fmt chunk and copy its contents into a WAVEFORMATEXTENSIBLE*/
	/*************************************************************************/
	findChunk(file,fourccFMT,chunkSize,chunkPosition);
	readChunkData(file,&wfx,chunkSize,chunkPosition);

	/************************************************************************/
	/* Locate the data chunk and read its contents into a buffer            */
	/************************************************************************/
	findChunk(file, fourccDATA,chunkSize,chunkPosition);
	BYTE* dataBuffer = new BYTE[chunkSize];
	readChunkData(file,dataBuffer,chunkSize, chunkPosition);

	/************************************************************************/
	/* Populate the XAUDIO2_BUFFER                                          */
	/************************************************************************/
	ZeroMemory(&buffer,sizeof(XAUDIO2_BUFFER));

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = dataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopBegin = 0;
	if(pLoopPlayback)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	else
		buffer.LoopCount = 0;
}
HRESULT SoundEffect::play()
{
	HRESULT hr= S_OK;
	hr = source->FlushSourceBuffers();
	hr = source->SubmitSourceBuffer(&buffer);
	hr = source->Start(0);
	playing = true;
	return hr;
}
HRESULT SoundEffect::stop()
{
	HRESULT hr = S_OK;
	hr = source->Stop();
	playing = false;
	return hr;
}
void SoundEffect::update()
{
}
HRESULT SoundEffect::findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
				hr = HRESULT_FROM_WIN32( GetLastError() );
			break;

		default:
			if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
				return HRESULT_FROM_WIN32( GetLastError() );            
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}
	return hr;
}
HRESULT SoundEffect::readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferOffset, NULL, FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );
	DWORD dwRead;
	if( 0 == ReadFile( hFile, buffer, bufferSize, &dwRead, NULL ) )
		hr = HRESULT_FROM_WIN32( GetLastError() );
	return hr;
}
HRESULT SoundEffect::initFile()
{
	HRESULT hr = S_OK;
	file = CreateFile(fileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);

	if (INVALID_HANDLE_VALUE == file)
		return HRESULT_FROM_WIN32( GetLastError() );

	if ( INVALID_SET_FILE_POINTER == SetFilePointer(file,0,NULL,FILE_BEGIN) )
		return HRESULT_FROM_WIN32( GetLastError() );
	return hr;
}

bool SoundEffect::isPlaying()
{
	return playing;
}