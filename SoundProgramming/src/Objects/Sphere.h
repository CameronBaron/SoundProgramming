#pragma once
#include "BaseObject.h"

class Sphere : public BaseObject
{
public:
	Sphere(vec3 pos, vec3 sc, float occ, float rev);
	~Sphere();

	virtual void Update();
};

