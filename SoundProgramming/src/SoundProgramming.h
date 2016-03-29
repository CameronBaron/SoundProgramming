#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "SoundClass.h"
#include "Objects\Cube.h"
#include <vector>

class SoundProgramming : public Application
{
public:
    virtual bool startup();
    virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void CreateCommonSounds();	// Sounds that wont get released after being used once.

	void UpdateGUI();

    FlyCamera m_camera;
	
	FMOD_RESULT result;

	FMOD::System* m_pFModSystem;

	FMOD::ChannelGroup* m_channelGroupMusic;
	FMOD::ChannelGroup* m_channelGroupSFX;

	FMOD_VECTOR m_listenerPosition;
	FMOD_VECTOR m_listenerVelocity;
	FMOD_VECTOR m_listenerForward;
	FMOD_VECTOR m_listenerUp;

	FMOD_VECTOR m_FMChannelPos = { 0, 0, 0 };
	FMOD_VECTOR m_FMChannelVel = { 0, 0, 0 };

	float gunCooldown = 2;
	float guntimer = 0;
	float lastFrameTime = 0;
	float dt = 0;

	bool GUIopened = true;
	bool keypress = false;

	SoundClass* bgSound;
	float sound1volume = 1;
	vec3 sound1Pos;
	bool sound1moving = true;

	SoundClass* bgSound2;
	float sound2volume = 1;
	FMOD_VECTOR sound2Pos;
	bool sound2moving = false;

	Cube* box;
	Cube* floor;
};

#endif //CAM_PROJ_H_