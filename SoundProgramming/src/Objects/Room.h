#pragma once
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "../Gizmos.h"
#include "../../deps/glm/glm/gtc/quaternion.hpp"
#include "../../deps/glm/glm/gtx/quaternion.hpp"

using namespace glm;

class Room
{
public:
	Room(vec3 a_pos, float a_scale, float a_wallOcc, float a_wallReverb, vec4 a_wallColour, float a_reverbRadius, float reverbOuterReverb);
	~Room();

	void Init(FMOD::System* a_FMsystem);
	void Update();
	void Draw();

	Cube* walls[5];
	vec4 wallColour = vec4(0.5f, 0.5f, 0.5f, 1);
	float wallOcclusion = 1;
	float wallReverb = 1;

	vec3 position = vec3(0);
	float scale = 1;
	quat rotation;

	mat4 localMatrix = mat4(1);
	mat4 worldMatrix = mat4(1);

	FMOD_VECTOR soundPosition = { 0, 0, 0 };
	FMOD_VECTOR reverbPosition = { 0, 0, 0 };
	float reverbInnerRadius = 1;
	float reverbOuterRadius = 5;
	FMOD_REVERB_PROPERTIES reverbProps = FMOD_PRESET_GENERIC;
};

