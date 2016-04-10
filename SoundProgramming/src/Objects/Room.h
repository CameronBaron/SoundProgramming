#pragma once
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "../Gizmos.h"
#include "../../deps/glm/glm/gtc/quaternion.hpp"
#include "../../deps/glm/glm/gtx/quaternion.hpp"
#include "../SoundClass.h"

using namespace glm;

class Room : public BaseObject
{
public:
	Room(vec3 a_pos, vec3 a_scale, float a_wallOcc, float a_wallReverb, vec4 a_wallColour, float a_reverbRadius, float reverbOuterReverb);
	~Room();

	void Init(FMOD::System* a_FMsystem);
	void Update();
	void Draw();

	FMOD_RESULT result;

	Cube* walls[5];
	vec4 wallColour = vec4(0.5f, 0.5f, 0.5f, 1);
	float wallOcclusion = 1;
	float wallReverb = 1;

	SoundClass* sound;
	const char* soundFilePath;
	FMOD::ChannelGroup* audioGroup;
	FMOD_VECTOR soundPosition = { 0, 0, 0 };
	FMOD_VECTOR reverbPosition = { 0, 0, 0 };
	float reverbInnerRadius = 1;
	float reverbOuterRadius = 5;

	FMOD::Reverb3D* reverb;
	FMOD_REVERB_PROPERTIES reverbProps = FMOD_PRESET_GENERIC;
};

