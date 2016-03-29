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
	BaseObject(vec3 pos, vec3 scale, float occlusion, float reverb) : position(pos), scale(scale), directOcclusion(occlusion), reverbOcclusion(reverb) {}
	~BaseObject() {}

	virtual void Init() {}
	virtual void Update() { UpdateModelMatrix(); }

	const vec3 GetPosition() { position.x = modelMatrix[3][0]; position.y = modelMatrix[3][1]; position.z = modelMatrix[3][2]; return position; }
	void SetPosition(vec3 pos) { position = pos; UpdateModelMatrix(); }
	void SetPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; UpdateModelMatrix(); }

	const quat GetRotation() { return rotation; }
	void SetRotation(quat rot) { rotation = rot; UpdateModelMatrix(); }

	const vec3 GetScale() { return scale; }
	void SetScale(vec3 s) { scale = s; UpdateModelMatrix(); }
	void SetScale(float x, float y, float z) { scale.x = x; scale.y = y; scale.z = z; UpdateModelMatrix(); }

	const vec4 GetColor() { return color; }
	void SetColor(vec4 c) { color = c; }
	void SetColor(float x, float y, float z, float w) { color.x = x; color.y = y; color.z = z; color.w = w; }

	void UpdateModelMatrix() { modelMatrix = glm::scale(vec3(scale)) * glm::toMat4(rotation) *  glm::translate(position); }
	const mat4 GetModelMatrix() { return modelMatrix; }

	std::string m_name;
	FMOD::Geometry* geometry;
protected:
	vec3 position;
	quat rotation;
	vec3 scale;
	vec4 color;

	mat4 modelMatrix;

	float directOcclusion;
	float reverbOcclusion;
	bool doubleSided;
};

