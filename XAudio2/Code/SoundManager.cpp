#include "SoundManager.h"

SoundManager* SoundManager::s_instance = NULL;

SoundManager::SoundManager()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_music			= NULL;
	m_masterVolume	= 0;
	m_left			= 0;
	m_right			= 0;


	/************************************************************************/
	/* Don't looks much to the world but here is where all the magic is done*/
	/************************************************************************/
	init();

	initListener();

	m_masterVoice->GetVolume(&m_masterVolume);
}

SoundManager::~SoundManager()
{
	if (m_music)
	{
		delete m_music;
		m_music = NULL;
	}
	if (m_masterVoice)
	{
		m_masterVoice->DestroyVoice();
		m_masterVoice = NULL;
	}
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

void SoundManager::update(Camera* p_gameCamera)
{
	m_masterVoice->SetVolume(m_masterVolume,0);
	updateListener(p_gameCamera);

	X3DAudioCalculate(m_X3DAudioInstance, &m_listener, &m_music->getEmitter(),
		X3DAUDIO_CALCULATE_MATRIX, &m_music->getDSPSettings());

	IXAudio2SourceVoice* voice = m_music->getSource();
	m_left = m_matrixCoefficients[0];
	m_right = m_matrixCoefficients[1];
	voice->SetOutputMatrix( m_masterVoice, 1, m_destChannels, m_matrixCoefficients);

	if(!m_music->isPlaying())
		m_music->play();
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

	m_destChannels = details.OutputFormat.Format.nChannels;

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, 
		m_X3DAudioInstance);

	m_matrixCoefficients	= new FLOAT32[1*details.OutputFormat.Format.nChannels];
	m_emitterAzimuths		= new FLOAT32[1];

	initPositionalSound();

	return hr;
}

float* SoundManager::getMasterVolume()
{
	return &m_masterVolume;
}

HRESULT SoundManager::initListener()
{
	HRESULT hr = S_OK;

	m_listener.Position = D3DXVECTOR3( 0, 0, 0);
	m_listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
	m_listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
	m_listener.Velocity = D3DXVECTOR3( 0, 0 ,0 );
	m_listener.pCone = NULL;
	//m_listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

	return hr;
}

HRESULT SoundManager::initEmitter(X3DAUDIO_EMITTER* p_emitter , X3DAUDIO_CONE* p_cone)
{
	HRESULT hr = S_OK;

	p_emitter->pCone = NULL;
	if (p_emitter->pCone)
	{
		p_emitter->pCone->InnerAngle = 0.0f;
		p_emitter->pCone->OuterAngle = 0.0f;
		p_emitter->pCone->InnerVolume = 0.0f;
		p_emitter->pCone->OuterVolume = 1.0f;
		p_emitter->pCone->InnerLPF = 0.0f;
		p_emitter->pCone->OuterLPF = 1.0f;
		p_emitter->pCone->InnerReverb = 0.0f;
		p_emitter->pCone->OuterReverb = 1.0f;

		p_emitter->InnerRadius = 2.0f;
		p_emitter->InnerRadiusAngle = X3DAUDIO_PI/4.0f;;
	}
	else
	{
		p_emitter->InnerRadius = 0.0f;
		p_emitter->InnerRadiusAngle = 0.0f;
	}

	p_emitter->Position = D3DXVECTOR3(30, -4.75f, 10);
	p_emitter->OrientFront = D3DXVECTOR3( 0, 0, 1 );
	p_emitter->OrientTop = D3DXVECTOR3( 0, 1, 0 );
	p_emitter->Velocity = D3DXVECTOR3(0, 0, 0);
	p_emitter->ChannelCount = 1;
	p_emitter->ChannelRadius = 1.0f;
	p_emitter->pChannelAzimuths = m_emitterAzimuths;

	p_emitter->pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	p_emitter->pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	p_emitter->pLPFDirectCurve = NULL; // use default curve
	p_emitter->pLPFReverbCurve = NULL; // use default curve
	p_emitter->pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	p_emitter->CurveDistanceScaler = 30.0f;
	p_emitter->DopplerScaler = 1.0f;

	return hr;
}

HRESULT SoundManager::initPositionalSound()
{
	HRESULT hr = S_OK;

	X3DAUDIO_EMITTER emitter;
	X3DAUDIO_CONE cone;
	initEmitter(&emitter, &cone);

	X3DAUDIO_DSP_SETTINGS dsp_settings;
	dsp_settings.SrcChannelCount = 1;
	dsp_settings.DstChannelCount = m_destChannels;
	dsp_settings.pMatrixCoefficients = m_matrixCoefficients;

	m_music = new SFX(emitter, dsp_settings, cone , m_soundDevice, 
		"../Sound/Techno_1.wav",true);

	return hr;
}

void SoundManager::updateListener( Camera* p_gameCamera )
{
	m_listener.Position = p_gameCamera->getPos();
	m_listener.OrientFront = p_gameCamera->getForward();
	m_listener.OrientTop = p_gameCamera->getUp();
	m_listener.Velocity = D3DXVECTOR3( 0, 0 ,0 );
	m_listener.pCone = NULL;
}

float* SoundManager::getLeftVolume()
{
	return &m_left;
}

float* SoundManager::getRightVolume()
{
	return &m_right;
}
