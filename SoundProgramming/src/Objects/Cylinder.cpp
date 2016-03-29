#include "Cylinder.h"



Cylinder::Cylinder(float a_radius, float a_halfLength, vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_reverb) :
	BaseObject(a_pos, a_scale, a_color, a_occ, a_reverb), m_radius(a_radius), m_halfLength(a_halfLength)
{
}


Cylinder::~Cylinder()
{
}

void Cylinder::Update()
{
	Gizmos::addCylinderFilled(m_position, m_radius, m_halfLength, 20, m_color, &m_modelMatrix);

	BaseObject::Update();
}
