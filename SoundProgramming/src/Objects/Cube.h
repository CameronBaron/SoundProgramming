#pragma once
#include "BaseObject.h"

class Cube : public BaseObject
{
public:
	Cube(vec3 pos, vec3 sc, float occ, float rev, bool filled);
	~Cube();

	virtual void Init();
	virtual void Update();

	void CreateVerts();

private:
	FMOD_RESULT result;

	FMOD_VECTOR tempPos;

	bool isFilled;
};

