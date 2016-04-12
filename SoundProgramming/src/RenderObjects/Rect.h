#pragma once
#include "RenderObject.h"
#include "../../deps/fmod/inc/fmod.hpp"
#include "../../deps/fmod/inc/fmod_errors.h"

class Rect : public RenderObject
{
public:
	Rect(std::string a_name, glm::vec3 a_pos, glm::vec3 a_size, glm::quat a_rot, const char* a_vertShader,
		const char* a_fragShader, float a_directOcclusion, float a_reverbOcclusion, Node* a_parent);
	~Rect();

	virtual void Init();
	virtual void Update();

	FMOD_RESULT result;

	FMOD::Geometry* m_geometry;
	
	float m_directOcclusion = 1;
	float m_reverbOcclusion = 1;

	glm::vec3 m_size;
};