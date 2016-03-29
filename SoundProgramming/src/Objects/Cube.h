#pragma once
#include "BaseObject.h"

class Cube : public BaseObject
{
public:
	Cube(vec3 pos, vec3 scale, vec4 color, float occ, float rev, bool filled);
	~Cube();

	virtual void Init();
	virtual void Update();

	void CreateVerts();

private:
	FMOD_RESULT result;

	bool isFilled;
};

