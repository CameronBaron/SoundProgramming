#include "Sphere.h"



Sphere::Sphere(float a_radius, vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_reverb) : 
	BaseObject(a_pos, a_scale, a_color, a_occ, a_reverb), m_radius(a_radius)
{
}

Sphere::~Sphere()
{
}

void Sphere::Update()
{
	Gizmos::addSphere(m_position, m_radius, 20, 20, m_color, &m_localMatrix);

	BaseObject::Update();
}
