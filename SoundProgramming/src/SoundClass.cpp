#include "SoundClass.h"


SoundClass::SoundClass(FMOD::System** mainSystemRef, FMOD::ChannelGroup* channelGroup, FMOD_VECTOR position, const char* filePath) : m_FModSysRef(mainSystemRef), m_channelGroupRef(channelGroup)
{
	SetDefaultValues();
	// Add to static list of sounds?

	result = (*m_FModSysRef)->createSound( filePath, FMOD_CREATESTREAM | FMOD_3D, 0, &m_audioClip);
}

SoundClass::~SoundClass()
{
	result = m_audioClip->release();
}

void SoundClass::Update()
{
	if (m_isPlaying)
	{
		result = m_channelRef->getPosition(&m_time, FMOD_TIMEUNIT_PCM);
	}
}

void SoundClass::FMODErrorCheck(FMOD_RESULT res)
{
	if (res != FMOD_OK)
	{
		printf("FMOD Error. (%d) %s\n", res, FMOD_ErrorString(res));
	}
}

void SoundClass::Play()
{
	//if (s_delayTimer <= 0)
	{
		if (!m_loop)
			result = m_audioClip->setMode(FMOD_LOOP_OFF);
		else
		{
			result = m_audioClip->setMode(FMOD_LOOP_NORMAL);
			result = m_audioClip->setLoopCount(-1);
		}
		result = (*m_FModSysRef)->playSound(m_audioClip, m_channelGroupRef, false, &m_channelRef);
		FMODErrorCheck(result);
		//update all needed vars
		m_isPlaying = true;
		m_mute = false;
	}
}

void SoundClass::Pause()
{
	result = m_channelRef->setPaused(true);
	FMODErrorCheck(result);
}

void SoundClass::PlayDelayed(float timeDelay)
{
	s_delayTimer = timeDelay;
}

void SoundClass::SetDefaultValues()
{
	m_audioClip = nullptr;
	m_channelRef = nullptr;

	m_bypassEffects = false;
	m_bypassListenerEffects = false;
	m_bypassReverbZones = false;
	m_ignoreListernerPause = false;
	m_isPlaying = true;
	m_isPlaying = false;
	m_loop = false;
	m_mute = false;
	m_playOnAwake = true;
	m_spatialize = false;
	m_priority = 128;
	
	m_dopplerLevel = 1;
	m_minDistance = 0.1f;
	m_maxDistance = 5000;
	m_pitch = 1;
	m_reverbZoneMix = 1;
	m_spatialBlend = 0;
	m_spread = 0;
	m_time = 0;
	m_volume = 1;
}

void SoundClass::Stop()
{
	result = m_channelRef->stop();
	m_isPlaying = false;
}

void SoundClass::UnPause()
{
	result = m_channelRef->setPaused(false);
	FMODErrorCheck(result);
}
