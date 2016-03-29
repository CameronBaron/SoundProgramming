#pragma once
#include "BaseObject.h"

class Cylinder : public BaseObject
{
public:
	Cylinder(float a_radius, float a_halfLength, vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_reverb);
	~Cylinder();

	virtual void Update();

	float m_radius, m_halfLength;
};

