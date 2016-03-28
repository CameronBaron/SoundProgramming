#pragma once
#include "gl_core_4_4.h"
#include "tinyobjloader\tiny_obj_loader.h"
#include <iostream>
#include <vector>
#include "../deps/glm/glm/glm.hpp"
#include "../deps/glm/glm/ext.hpp"

struct OpenGLInfo
{
	OpenGLInfo() { m_VAO = m_VBO = m_IBO = m_index_count = 0; }
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

class OBJLoader
{
public:
	OBJLoader(const char* name, const char* objFilePath, const char* vertShaderfilePath, const char* fragShaderFilePath);
	~OBJLoader();

	void Init();
	void LoadFile();
	void PrintMeshInfo();
	void CreateBuffers();
	void DrawElements();
	unsigned int LoadShaderFromFile(const char * _filePath, unsigned int _shaderType);

	const char* m_OBJFilePath;
	const char* m_vertShaderFilePath;
	const char* m_fragShaderFilePath;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<OpenGLInfo> gl_info;

	unsigned int m_programID;
	const char* name;
	glm::mat4 modelMatrix;
};

