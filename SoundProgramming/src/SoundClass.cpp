#include "SoundClass.h"
#include "Gizmos.h"
#include "../deps/glm/glm/glm.hpp"
#include <algorithm>

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
		result = m_channelRef->getPosition(&m_time, FMOD_TIMEUNIT_PCM);
		m_channelRef->set3DAttributes(&m_channelPosition, &m_channelVelocity);

		//Gizmos::addSphere(glm::vec3(m_channelPosition.x, m_channelPosition.y, m_channelPosition.z), 0.1f, 10, 10, glm::vec4(1, 0, 0, 1));
	}

	result = dsp_fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftParameter, &len, s, 256);	

	fftHeights.resize(m_sampleSize);

	for (int channel = 0; channel < fftParameter->numchannels; channel++) // Two channels Left and Right but can also allow for surround or mono
	{
		for (int bin = 0; bin < m_sampleSize; bin++)
		{
			float val = fftParameter->spectrum[channel][bin];
			if (channel == 0)
			{
				fftHeights[bin] = val;
			}
			else
			{
				fftHeights[bin] += val;
				//fftHeights[bin] /= 2;
			}
		}
	}

#pragma region Beat Detection

	// Lowest frequency sounds in sample 1 (eg. bass/drums)
	beatIntensity = 0;
	for (int i = 0; i < 5; i++)
	{
		if (beatIntensity < fftHeights[i])
			beatIntensity = fftHeights[i];
	}

#pragma endregion

	FMODErrorCheck();
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
			result = m_audioClip->setMode(FMOD_LOOP_OFF);
		}
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
		//result = (*m_FModSysRef)->getMasterChannelGroup(&m_master_channelGroupRef);
		result = m_channelGroupRef->getDSP(FMOD_CHANNELCONTROL_DSP_TAIL, &dsp_tail);
		result = dsp_tail->addInput(dsp_reverb);
		result = dsp_reverb->setActive(true);		

		result = m_channelRef->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &channel_dsp_head);
		result = dsp_reverb->addInput(channel_dsp_head);
		
		result = dsp_reverb->setBypass(dsp_reverbBypass);
		/* Has the benifit of not disabling all inputs as SetActive would, and reverb process is not called, saving CPU */
#pragma endregion

#pragma region Spectrum Info
		result = (*m_FModSysRef)->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp_fft);
		result = m_channelRef->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, dsp_fft); // Add it to the tail, which gets the info before any 3D/Reverb/Volume effects applied.
		result = dsp_fft->setActive(true);
#pragma endregion

		UnPause();
		m_isPlaying = true;
		m_mute = false;
		FMODErrorCheck();
	}
}

void SoundClass::Pause()
{
	result = m_channelRef->setPaused(true);
	FMODErrorCheck();
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

	FMODErrorCheck();
}

void SoundClass::UnPause()
{
	result = m_channelRef->setPaused(false);
	FMODErrorCheck();
}
