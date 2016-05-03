#include "SoundClass.h"
#include "Gizmos.h"
#include "../deps/glm/glm/glm.hpp"
#include <algorithm>

bool myPred(float a, float b) { return a < b; }

SoundClass::SoundClass(FMOD::System** mainSystemRef, FMOD::ChannelGroup* channelGroup, FMOD_VECTOR position, const char* filePath) : m_FModSysRef(mainSystemRef), m_channelGroupRef(channelGroup)
{
	SetDefaultValues();

	result = (*m_FModSysRef)->createSound( filePath, FMOD_CREATESTREAM | FMOD_3D, 0, &m_audioClip);

	FMODErrorCheck();

	numOfBars = 32;
	barVals = new float[numOfBars];
}

SoundClass::~SoundClass()
{
	result = m_audioClip->release();
	fftHeights.clear();
	delete[] barVals;
}

void SoundClass::Update()
{
	result = dsp_reverb->setBypass(dsp_reverbBypass);

	if (m_isPlaying)
	{
		m_channelRef->getPosition(&m_time, FMOD_TIMEUNIT_PCM);
		m_channelRef->set3DAttributes(&m_channelPosition, &m_channelVelocity);
	}

	dsp_fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftParameter, &len, s, 256);	

	fftHeights.resize(m_sampleSize);
	std::fill(fftHeights.begin(), fftHeights.end(), 0.f);

	std::transform(fftHeights.begin(), fftHeights.end(), fftParameter->spectrum[0], fftHeights.begin(), std::plus<float>());
	for (int channel = 1; channel < fftParameter->numchannels;) // Two channels Left and Right but can also allow for surround or mono
	{
		std::transform(fftHeights.begin(), fftHeights.end(), fftParameter->spectrum[++channel], fftHeights.begin(), std::plus<float>());
	}

#pragma region Beat Detection

	// Lowest frequency sounds in sample 1 (eg. bass/drums)
	beatIntensity = 0;
	beatIntensity = *std::max_element(fftHeights.begin(), fftHeights.begin() + 5);

#pragma endregion
}

void SoundClass::FMODErrorCheck()
{
	if (result != FMOD_OK)
	{
		printf("FMOD Error. (%d) %s\n", result, FMOD_ErrorString(result));
	}
}

void SoundClass::Play()
{
	//if (s_delayTimer <= 0)
	{
		if (!m_loop)
		{
			m_audioClip->setMode(FMOD_LOOP_OFF);
		}
		else
		{
			m_audioClip->setMode(FMOD_LOOP_NORMAL);
			m_audioClip->setLoopCount(-1);
		}
		(*m_FModSysRef)->playSound(m_audioClip, m_channelGroupRef, true, &m_channelRef);
		//update all needed vars

#pragma region Reverb Example
		(*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dsp_reverb);
		m_channelGroupRef->getDSP(FMOD_CHANNELCONTROL_DSP_TAIL, &dsp_tail);
		dsp_tail->addInput(dsp_reverb);
		dsp_reverb->setActive(true);		

		m_channelRef->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channel_dsp_head);
		dsp_reverb->addInput(channel_dsp_head);
		
		dsp_reverb->setBypass(dsp_reverbBypass);
		/* Has the benifit of not disabling all inputs as SetActive would, and reverb process is not called, saving CPU */
#pragma endregion

#pragma region Spectrum Info
		(*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp_fft);
		m_channelRef->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, dsp_fft); // Add it to the tail, which gets the info before any 3D/Reverb/Volume effects applied.
		dsp_fft->setActive(true);
#pragma endregion

		UnPause();
		m_isPlaying = true;
		m_mute = false;
	}
}

void SoundClass::Pause()
{
	m_channelRef->setPaused(true);
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
	m_channelRef->stop();
	m_isPlaying = false;
}

void SoundClass::UnPause()
{
	m_channelRef->setPaused(false);
}
