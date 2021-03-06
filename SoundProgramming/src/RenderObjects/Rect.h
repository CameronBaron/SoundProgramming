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

	void LoadTextureData(int a_dataSize, float* a_data);
	void UpdateTexData(int a_dataSize, float* a_data);

	void LoadRawData();

	FMOD_RESULT result;

	FMOD::Geometry* m_geometry;
	FMOD_VECTOR m_geoForward{ 0, 0, 1 };
	FMOD_VECTOR m_geoUp{ 0, 1, 0 };
	
	float m_directOcclusion = 1;
	float m_reverbOcclusion = 1;

	glm::vec3 m_size;
	unsigned int m_equaliserTex; // 1D texture for sound equaliser
	GLuint texLoc;

//Lighting
	unsigned int lightPosLoc = 0;
	unsigned int lightColorLoc = 0;
	unsigned int lightAmbientLoc = 0;
	unsigned int lightConstantAttLoc = 0;
	unsigned int lightLinearAttLoc = 0;
	unsigned int lightExpAttLoc = 0;
	unsigned int worldPosLoc = 0;
};