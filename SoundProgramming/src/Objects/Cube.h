#pragma once
#include "BaseObject.h"

class Cube : public BaseObject
{
public:
	Cube(vec3 pos, vec3 scale, vec4 color, float occ, float rev, bool filled);
	Cube(vec3 pos, vec3 scale, vec4 color, float occ, float rev, bool filled, BaseObject* a_parent);
	~Cube();

	virtual void Init();
	virtual void Update();

private:
	FMOD_RESULT result;
	FMOD_VECTOR* cube;
	FMOD_VECTOR up;
	FMOD_VECTOR forward;

	bool isFilled;
};

