#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "SoundClass.h"
#include "Objects\Room.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include "OBJLoader.h"
#include <vector>
#include "RenderObjects\OpenBox.h"

class SoundProgramming : public Application
{
public:
    virtual bool startup();
    virtual void shutdown();
    virtual bool update();
    virtual void draw();
	void DrawGUI();

	void SongFilePaths();

	void UpdateGUI();

    FlyCamera m_camera;
	
	FMOD_RESULT result;

	FMOD::System* m_pFModSystem;

	FMOD::ChannelGroup* m_channelGroupMusic;
	FMOD::ChannelGroup* m_channelGroupSFX;

	FMOD::Reverb3D* m_reverb3D;

	FMOD_VECTOR m_listenerPosition;
	FMOD_VECTOR m_listenerVelocity;
	FMOD_VECTOR m_listenerForward;
	FMOD_VECTOR m_listenerUp;
	float dopplerLevel = 0;			// Effects all sound pitch depending on listener velocity

	float gunCooldown = 2;
	float guntimer = 0;
	float lastFrameTime = 0;
	float dt = 0;

	bool GUIopened = true;
	bool keypress = false;

	const char* songFiles[5];
	FMOD_REVERB_PROPERTIES soundProps[5];

	OpenBox* room;
	float a = 1;
};

#endif //CAM_PROJ_H_