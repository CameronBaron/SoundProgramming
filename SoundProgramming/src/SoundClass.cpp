#include "SoundClass.h"
#include "Gizmos.h"
#include "../deps/glm/glm/glm.hpp"

SoundClass::SoundClass(FMOD::System** mainSystemRef, FMOD::ChannelGroup* channelGroup, FMOD_VECTOR position, const char* filePath) : m_FModSysRef(mainSystemRef), m_channelGroupRef(channelGroup)
{
	SetDefaultValues();
	// Add to static list of sounds?

	result = (*m_FModSysRef)->createSound( filePath, FMOD_CREATESTREAM | FMOD_3D, 0, &m_audioClip);
	specLeft = new float[m_sampleSize];
	specRight = new float[m_sampleSize];
}

SoundClass::~SoundClass()
{
	result = m_audioClip->release();
}

void SoundClass::Update()
{
	result = dsp_reverb->setBypass(dsp_chorusBypass);
	result = dsp_reverb->setBypass(dsp_reverbBypass);

	if (m_isPlaying)
	{
		result = m_channelRef->getPosition(&m_time, FMOD_TIMEUNIT_PCM);
		m_channelRef->set3DAttributes(&m_channelPosition, &m_channelVelocity);

		Gizmos::addSphere(glm::vec3(m_channelPosition.x, m_channelPosition.y, m_channelPosition.z), 0.1f, 10, 10, glm::vec4(1, 0, 0, 1));
	}
	
	result = dsp_fft->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_TRIANGLE);
	result = dsp_fft->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, windowSize);
	result = dsp_fft->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, 0, 0);
	result = dsp_fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftParameter, &len, s, 256);

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
		result = (*m_FModSysRef)->playSound(m_audioClip, m_channelGroupRef, true, &m_channelRef);
		//update all needed vars

#pragma region Reverb Example
		FMOD::ChannelGroup* m_master_channelGroupRef;
		result = (*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dsp_reverb);
		result = (*m_FModSysRef)->getMasterChannelGroup(&m_master_channelGroupRef);
		result = m_master_channelGroupRef->getDSP(FMOD_CHANNELCONTROL_DSP_TAIL, &dsp_tail);
		result = dsp_tail->addInput(dsp_reverb);
		result = dsp_reverb->setActive(true);		

		result = m_channelRef->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channel_dsp_head);
		result = dsp_reverb->addInput(channel_dsp_head);
		
		result = dsp_reverb->setBypass(dsp_reverbBypass);
		/* Has the benifit of not disabling all inputs as SetActive would, and reverb process is not called, savinmg CPU */
#pragma endregion

#pragma region Chorus DSP
		result = (*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dsp_chorus);
		result = dsp_tail->addInput(dsp_chorus);
		result = dsp_chorus->setActive(true);
		result = dsp_chorus->addInput(channel_dsp_head);

		result = dsp_reverb->setBypass(dsp_chorusBypass);
#pragma endregion


#pragma region Spectrum Attempt
		result = (*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp_fft);
		result = m_master_channelGroupRef->addDSP(1, dsp_fft);
		result = dsp_fft->setActive(true);
#pragma endregion

		FMODErrorCheck(result);
		UnPause();
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
