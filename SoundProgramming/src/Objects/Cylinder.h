#pragma once
#include "BaseObject.h"

class Cylinder : public BaseObject
{
public:
	Cylinder(vec3 pos, vec3 sc, float occ, float rev);
	~Cylinder();

	virtual void Update();
};

