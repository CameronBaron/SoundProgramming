#pragma once
#include "../../deps/glm/glm/glm.hpp"
#include "../../deps/glm/glm/ext.hpp"

using namespace glm;

class BaseObject
{
public:
	BaseObject();
	~BaseObject();

	const vec3 GetPosition() { position.x = modelMatrix[3][0]; position.y = modelMatrix[3][1]; position.z = modelMatrix[3][2]; return position; }
	void SetPosition(vec3 pos) { position = pos; UpdateModelMatrix(); }
	void SetPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; UpdateModelMatrix(); }

	const quat GetRotation() { return rotation; }
	void SetRotation(quat rot) { rotation = rot; UpdateModelMatrix(); }

	const float GetScale() { return scale; }
	void SetScale(float s) { scale = s; UpdateModelMatrix(); }

	virtual void Update() { UpdateModelMatrix(); }
	virtual void Draw();

	void UpdateModelMatrix() { modelMatrix = glm::scale(vec3(scale)) * glm::toMat4(rotation) *  glm::translate(position); }
	const mat4 GetModelMatrix() { return modelMatrix; }

protected:
	vec3 position;
	quat rotation;
	float scale;

	mat4 modelMatrix;
};

