#include "BaseObject.h"

BaseObject::BaseObject(vec3 a_pos, vec3 a_scale, vec4 a_color) : m_position(a_pos), m_scale(a_scale), m_directOcclusion(1), m_reverbOcclusion(1)
{
	m_color = a_color;
	GetRoot().AddChild(this);
}

BaseObject::BaseObject(vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occlusion, float a_reverb) : m_position(a_pos), m_scale(a_scale), m_directOcclusion(a_occlusion), m_reverbOcclusion(a_reverb)
{
	m_color = a_color;
	GetRoot().AddChild(this);
}

BaseObject::~BaseObject()
{
	if (m_parent != nullptr)
		m_parent->RemoveChild(this);

	for (BaseObject* child : m_children)
		child->m_parent = nullptr;
}

void BaseObject::Init()
{
}

void BaseObject::Update()
{
	// Update geometry position here
	FMOD_VECTOR tempPos = { m_position.x, m_position.y, m_position.z };
	m_geometry->setPosition(&tempPos);

	FMOD_VECTOR fscale = { m_scale.x, m_scale.y, m_scale.z };
	m_geometry->setScale(&fscale);

	//FMOD_VECTOR frot;
	//m_geometry->setRotation();

	UpdateWorldMatrix();
}

//---------------------PARENT/CHILD-----------------//

BaseObject& BaseObject::GetRoot()
{
	static BaseObject root(true);
	return root;
}

void BaseObject::SetParent(BaseObject* parent)
{
	m_parent = parent;
}

void BaseObject::AddChild(BaseObject* childToAdd)
{
	auto iter = std::find(m_children.begin(), m_children.end(), childToAdd);
	if (iter == m_children.end())
	{
		childToAdd->SetParent(this);
		m_children.push_back(childToAdd);
		return;
	}
}

void BaseObject::RemoveChild(BaseObject* childToRemove)
{
	auto iter = std::find(m_children.begin(), m_children.end(), childToRemove);
	if (iter != m_children.end())
	{
		childToRemove->m_parent = nullptr;
		m_children.erase(iter);
		return;
	}
}

//----------------------POSITION-------------------//
const vec3 BaseObject::GetPosition()
{
	m_position.x = m_localMatrix[3][0]; 
	m_position.y = m_localMatrix[3][1]; 
	m_position.z = m_localMatrix[3][2]; 

	return m_position;
}

void BaseObject::SetPosition(vec3 a_pos)
{
	m_position = a_pos; 
}

void BaseObject::SetPosition(float x, float y, float z)
{
	m_position.x = x; 
	m_position.y = y; 
	m_position.z = z;
}

//----------------------ROTATION-------------------//

void BaseObject::SetRotation(vec3 a_rot)
{
	m_rotation = a_rot;
}

//----------------------SCALE----------------------//

void BaseObject::SetScale(vec3 a_scale)
{
	m_scale = a_scale;
}

void BaseObject::SetScale(float x, float y, float z)
{
	m_scale.x = x; 
	m_scale.y = y; 
	m_scale.z = z;
}

//----------------------COLOR----------------------//

void BaseObject::SetColor(vec4 a_color)
{
	m_color = a_color;
}

void BaseObject::SetColor(float x, float y, float z, float w)
{
	m_color.x = x; 
	m_color.y = y; 
	m_color.z = z; 
	m_color.w = w;
}

//----------------------MATRIX---------------------//

void BaseObject::UpdateWorldMatrix()
{
	m_localMatrix = glm::scale(vec3(m_scale)) * glm::rotate(m_rotationMat,0.0f, vec3(1,0,0)) * glm::translate(m_position);
	
	if (m_parent != nullptr)
		m_worldMatrix = m_localMatrix * m_parent->m_localMatrix;
	else
		m_worldMatrix = m_localMatrix;

	for (BaseObject* child : m_children)
		child->Update();
}

//----------------------RETURN ROOT----------------//
BaseObject::BaseObject(bool root)
{
}
