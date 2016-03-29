#pragma once
#include "BaseObject.h"

class Sphere : public BaseObject
{
public:
	Sphere(float a_radius, vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_reverb);
	~Sphere();

	virtual void Update();

	float m_radius;
};

