#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include <fmod.hpp>
#include <fmod_errors.h>

class SoundProgramming : public Application
{
public:
    virtual bool startup();
    virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void createSound(FMOD::Sound** pSound, const char* pFile);
	void playSound(FMOD::Sound* pSound, bool bLoop = false);
	void releaseSound(FMOD::Sound* pSound);

    FlyCamera m_camera;

	FMOD_RESULT result;

	FMOD::System* m_pFModSystem;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
	FMOD::ChannelGroup* m_pChannelGroup;

	FMOD_VECTOR m_FMposition;
	FMOD_VECTOR m_FMvelocity;
	FMOD_VECTOR m_FMforward;
	FMOD_VECTOR m_FMup;

	FMOD_VECTOR m_FMChannelPos = { 0, 0, 0 };
	FMOD_VECTOR m_FMChannelVel = { 0, 0, 0 };
};

#endif //CAM_PROJ_H_