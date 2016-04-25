#pragma once
#include "Rect.h"
#include "../Gizmos.h"
#include "../SoundClass.h"

class Camera;

class OpenBox : public Node
{
public:
	OpenBox(glm::vec3 a_pos, glm::vec3 a_scale, float a_wallOcc, float a_wallReverb, float a_reverbRadius, float a_reverbOuterReverb, const char* a_soundFilePath);
	~OpenBox();

	void Init(FMOD::System* a_FMsystem);
	void Update();
	void Draw(Camera* a_camera);
	void DrawGUI();
	void InitReverbPresets();

	Rect* m_walls[5];
	float m_wallOcclusion = 1;
	float m_wallReverb = 1;

	FMOD_RESULT result;

	FMOD::ChannelGroup* m_audioGroup = nullptr;
	const char* m_audioFilePath = "";

	SoundClass* m_sound;
	float m_volume = 1;
	FMOD_VECTOR m_soundPosition = { 0, 0, 0 };
	FMOD_VECTOR m_reverbPosition = { 0, 0, 0 };
	float m_reverbInnerRadius = 1;
	float m_reverbOuterRadius = 5;

	FMOD::Reverb3D* m_reverb = nullptr;
	FMOD_REVERB_PROPERTIES m_reverbProps = FMOD_PRESET_OFF;
	FMOD_REVERB_PROPERTIES* reverbs;
	const char** properties;
	int currProp;

	const char* m_cubeVertShader = "./data/Shaders/shaders.vert";
	const char* m_cubeFragShader = "./data/Shaders/shaders.frag";
};

