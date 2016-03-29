#pragma once
#include "../../deps/glm/glm/glm.hpp"
#include "../../deps/glm/glm/ext.hpp"
#include "../Gizmos.h"
#include <fmod.hpp>
#include <fmod_errors.h>

using namespace glm;

class BaseObject
{
public:
	BaseObject(vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occlusion, float a_reverb) : m_position(a_pos), m_scale(a_scale), m_directOcclusion(a_occlusion), m_reverbOcclusion(a_reverb) { m_color = a_color; }
	~BaseObject() {}

	virtual void Init() {}
	virtual void Update() 
	{ 
		// Update geometry position here
		FMOD_VECTOR tempPos = { m_position.x, m_position.y, m_position.z };
		m_geometry->setPosition(&tempPos);

		FMOD_VECTOR fscale = { m_scale.x, m_scale.y, m_scale.z };
		m_geometry->setScale(&fscale);
		UpdateModelMatrix(); 
	}

	const vec3 GetPosition() { m_position.x = m_modelMatrix[3][0]; m_position.y = m_modelMatrix[3][1]; m_position.z = m_modelMatrix[3][2]; return m_position; }
	void SetPosition(vec3 a_pos) { m_position = a_pos; UpdateModelMatrix(); }
	void SetPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; UpdateModelMatrix(); }

	const quat GetRotation() { return m_rotation; }
	void SetRotation(quat a_rot) { m_rotation = a_rot; UpdateModelMatrix(); }

	const vec3 GetScale() { return m_scale; }
	void SetScale(vec3 a_scale) { m_scale = a_scale; UpdateModelMatrix(); }
	void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; UpdateModelMatrix(); }

	const vec4 GetColor() { return m_color; }
	void SetColor(vec4 a_color) { m_color = a_color; }
	void SetColor(float x, float y, float z, float w) { m_color.x = x; m_color.y = y; m_color.z = z; m_color.w = w; }

	void UpdateModelMatrix() { m_modelMatrix = glm::scale(vec3(m_scale)) * glm::toMat4(m_rotation) *  glm::translate(m_position); }
	const mat4 GetModelMatrix() { return m_modelMatrix; }

	std::string m_name;
	FMOD::Geometry* m_geometry;
	int m_maxPolygons;
	int m_maxVerts;

protected:
	vec3 m_position;
	quat m_rotation;
	vec3 m_scale;
	vec4 m_color;

	mat4 m_modelMatrix;

	float m_directOcclusion;
	float m_reverbOcclusion;
	bool m_doubleSided;
};

