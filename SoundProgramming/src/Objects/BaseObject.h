#pragma once
#include "../../deps/glm/glm/glm.hpp"
#include "../../deps/glm/glm/ext.hpp"
#include "../Gizmos.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>

using namespace glm;

struct Vertex
{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
};

class BaseObject
{
public:
	BaseObject(vec3 a_pos, vec3 a_scale, vec4 a_color);
	BaseObject(vec3 a_pos, vec3 a_scale, vec4 a_color, float a_occlusion, float a_reverb);
	~BaseObject();

	static BaseObject& GetRoot();
	void SetParent(BaseObject* parent);
	void AddChild(BaseObject* childToAdd);
	void RemoveChild(BaseObject* childToRemove);

	virtual void Init();
	virtual void Update();

	const vec3 GetPosition();
	void SetPosition(vec3 a_pos);
	void SetPosition(float x, float y, float z);

	const vec3 GetRotation() { return m_rotation; }
	void SetRotation(vec3 a_rot);

	const vec3 GetScale() { return m_scale; }
	void SetScale(vec3 a_scale);
	void SetScale(float x, float y, float z);

	const vec4 GetColor() { return m_color; }
	void SetColor(vec4 a_color);
	void SetColor(float x, float y, float z, float w);

	void UpdateWorldMatrix();
	const mat4 GetWorldMatrix() { return m_localMatrix; }

	std::string m_name;
	FMOD::Geometry* m_geometry;
	int m_maxPolygons;
	int m_maxVerts;

protected:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;
	vec4 m_color;

	BaseObject(bool root);

	BaseObject* m_parent;
	std::vector<BaseObject*> m_children;
	mat4 m_localMatrix;
	mat4 m_worldMatrix;
	mat4 m_rotationMat = mat4(1);

	float m_directOcclusion;
	float m_reverbOcclusion;
	bool m_doubleSided;

};

