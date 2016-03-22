#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "SoundClass.h"

class SoundProgramming : public Application
{
public:
    virtual bool startup();
    virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void createSound(FMOD::Sound** pSound, const char* pFile);
	void playSound(FMOD::Sound* pSound, FMOD_VECTOR pPos, FMOD_VECTOR pVel, float dt, bool bLoop = false);
	void releaseSound(FMOD::Sound* pSound);

	void UpdateGUI();

    FlyCamera m_camera;
	
	FMOD_RESULT result;

	FMOD::System* m_pFModSystem;
	//FMOD::Studio::System* m_pFModStudioSystem;
	FMOD::Sound* m_pSound;
	FMOD::Sound* m_gunSound;
	FMOD::ChannelGroup* m_channelGroupMusic;
	FMOD::ChannelGroup* m_channelGroupSFX;
	FMOD::DSP* m_pEchoDSP;

	FMOD_VECTOR m_FMposition;
	FMOD_VECTOR m_FMvelocity;
	FMOD_VECTOR m_FMforward;
	FMOD_VECTOR m_FMup;

	FMOD_VECTOR m_FMChannelPos = { 0, 0, 0 };
	FMOD_VECTOR m_FMChannelVel = { 0, 0, 0 };

	float gunCooldown = 2;
	float guntimer = 0;
	float lastFrameTime = 0;
	float dt = 0;

};

#endif //CAM_PROJ_H_