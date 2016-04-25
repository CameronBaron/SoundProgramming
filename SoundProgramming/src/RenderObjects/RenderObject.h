#pragma once
#include "../gl_core_4_4.h"
#include "Node.h"

class Vert
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

class RenderObject : public Node
{
public:
	RenderObject(std::string a_name, glm::vec3 a_pos, glm::quat a_rot, const char* a_vertShader, const char* a_fragShader, Node* a_parent);
	~RenderObject();

	virtual void Init();
	virtual void Update();
	virtual void CreateOpenGLBuffers();
	virtual void DrawElements();

	unsigned int LoadShaderFromFile(const char * _filePath, unsigned int _shaderType);

	std::string m_name;

	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int m_programID;

	const char* m_vertShaderFilePath;
	const char* m_fragShaderFilePath;

	float* m_vertBuffer;
	unsigned int m_vertexCount;
	unsigned int* m_indices;
	unsigned int m_indicesCount;

	float* m_texcoordBuffer;
	unsigned int m_texcoordCount;
	float* m_normalBuffer;
	unsigned int m_normalCount;
	float* arrays;
	unsigned int arraySize;


protected:

};
