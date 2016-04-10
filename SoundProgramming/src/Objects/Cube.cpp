#include "Cube.h"



Cube::Cube(vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_rev, bool a_filled) : BaseObject(a_pos, a_scale, a_color, a_occ, a_rev)
{
	isFilled = a_filled;
}

Cube::Cube(vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occ, float a_rev, bool a_filled, BaseObject* a_parent) : BaseObject(a_pos, a_scale, a_color, a_occ, a_rev)
{
	isFilled = a_filled;
	m_parent->RemoveChild(this);
	SetParent(a_parent);
	m_parent->AddChild(this);
}


Cube::~Cube()
{
}

void Cube::Init()
{	
	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
	{
		{  1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x, -1 * m_scale.y,  1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z }, //+X face
		{ -1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z },{ -1 * m_scale.x, -1 * m_scale.y,  1 * m_scale.z },{ -1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z },{ -1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z }, //-X face
		{ -1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z },{ -1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z }, //+Y face
		{ -1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x, -1 * m_scale.y,  1 * m_scale.z },{ -1 * m_scale.x, -1 * m_scale.y,  1 * m_scale.z }, //-Y face
		{ -1 * m_scale.x, -1 * m_scale.y,  1 * m_scale.z },{ -1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y,  1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z }, //+Z face
		{ -1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z },{ -1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x,  1 * m_scale.y, -1 * m_scale.z },{  1 * m_scale.x, -1 * m_scale.y, -1 * m_scale.z }, //-Z face
	};
	int pi = 0;
	for (int i = 0; i < 6; ++i)
	{
		result = m_geometry->addPolygon(m_directOcclusion, m_reverbOcclusion, 1, 4, cube + (4 * i), &pi); // pointer arithmetic to get face i
	}
	m_geometry->getRotation(&forward, &up);
	//m_geometry->setRotation()
}

void Cube::Update()
{	
	m_rotation = vec3(1);
	BaseObject::Update();

	if (isFilled)
		Gizmos::addAABBFilled(m_position, m_scale, m_color, &m_worldMatrix);
	else
		Gizmos::addAABB(m_position, m_scale, m_color, &m_localMatrix);
}
