#include "SoundManager.h"

SoundManager* SoundManager::s_instance = NULL;

SoundManager::SoundManager()
{
	m_soundDevice = NULL;
	m_masterVoice = NULL;
	m_masterVolume = 0;

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


	playSound("../Sound/Techno_1.wav");

	return hr;
}

void SoundManager::playSound( string p_filePath )
{
	if (FAILED ( initFile(p_filePath) ) )
		return; //Bad file

	DWORD chunkSize, chunkPosition, fileType;
	if (FAILED (findChunk( m_file, fourccRIFF, chunkSize, chunkPosition )) )
		return; //Cannot find the right chunk bad sound file

	if( FAILED (readChunkData( m_file, &fileType, sizeof(DWORD), chunkPosition)) )
		return; //Cannot read that data file bad sound file

	if (fileType != fourccWAVE)
		return; //Something is wrong with the sound file
	
	/*************************************************************************/
	/* Locate the fmt chunk and copy its contents into a WAVEFORMATEXTENSIBLE*/
	/*************************************************************************/
	if (FAILED (findChunk(m_file,fourccFMT,chunkSize,chunkPosition)) )
		return;
	if (FAILED (readChunkData(m_file,&m_wfx,chunkSize,chunkPosition)) )
		return;

	/************************************************************************/
	/* Locate the data chunk and read its contents into a buffer            */
	/************************************************************************/
	if (FAILED (findChunk(m_file, fourccDATA,chunkSize,chunkPosition)) )
		return;
	BYTE* dataBuffer = new BYTE[chunkSize];
	if (FAILED (readChunkData(m_file,dataBuffer,chunkSize, chunkPosition)) )
		return;

	/************************************************************************/
	/* Populate the XAUDIO2_BUFFER                                          */
	/************************************************************************/
	ZeroMemory(&m_buffer,sizeof(XAUDIO2_BUFFER));
	m_buffer.AudioBytes = chunkSize;
	m_buffer.pAudioData = dataBuffer;
	m_buffer.Flags = XAUDIO2_END_OF_STREAM;
	m_buffer.LoopBegin = 0;
	m_buffer.LoopCount = 0;

	/************************************************************************/
	/* Create the sound file's source voice									*/
	/************************************************************************/
	if (FAILED ( m_soundDevice->CreateSourceVoice(&m_sourceVoice,(WAVEFORMATEX*)&m_wfx,0,
		XAUDIO2_DEFAULT_FREQ_RATIO,NULL,NULL,NULL)) )
		return;	//Input parameters are wrong

	if (FAILED (m_sourceVoice->FlushSourceBuffers()))
		return; //Cannot flush the buffer, bad buffer

	if (FAILED (m_sourceVoice->SubmitSourceBuffer(&m_buffer)) )
		return; //Cannot submit the source buffer, bad buffer

	if (FAILED (m_sourceVoice->Start(0)) )
		return; //Cannot start the sound
}

HRESULT SoundManager::findChunk(HANDLE p_file, DWORD p_fourcc, DWORD& p_dwChunkSize, 
								DWORD& p_dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( p_file, 0, NULL, FILE_BEGIN ) )
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
		if( 0 == ReadFile( p_file, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		if( 0 == ReadFile( p_file, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if( 0 == ReadFile( p_file, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
				hr = HRESULT_FROM_WIN32( GetLastError() );
			break;

		default:
			if( INVALID_SET_FILE_POINTER == SetFilePointer( p_file, dwChunkDataSize, 
				NULL, FILE_CURRENT ) )
				return HRESULT_FROM_WIN32( GetLastError() );            
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == p_fourcc)
		{
			p_dwChunkSize = dwChunkDataSize;
			p_dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) 
			return S_FALSE;
	}
	return hr;
}
HRESULT SoundManager::readChunkData(HANDLE p_file, void* p_buffer, DWORD p_bufferSize, 
									DWORD p_bufferOffset)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( p_file, p_bufferOffset, NULL, 
		FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );
	DWORD dwRead;
	if( 0 == ReadFile( p_file, p_buffer, p_bufferSize, &dwRead, NULL ) )
		hr = HRESULT_FROM_WIN32( GetLastError() );
	return hr;
}

HRESULT SoundManager::initFile(string p_filePath)
{
	HRESULT hr = S_OK;

	m_file = CreateFile(p_filePath.c_str(), GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 
		0, NULL);

	if( m_file == INVALID_HANDLE_VALUE)
		return S_FALSE;

	if( SetFilePointer( m_file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER )
		return S_FALSE;

	return hr;
}

float* SoundManager::getMasterVolume()
{
	return &m_masterVolume;
}
