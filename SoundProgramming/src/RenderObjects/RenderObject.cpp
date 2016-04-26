#include "RenderObject.h"
#include <string>
#include <iostream>

using namespace std;
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void _check_gl_error(const char *file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		cerr << "GL_" << error.c_str() << " - " << file << ":" << line << endl;
		err = glGetError();
	}
}

RenderObject::RenderObject(std::string a_name, glm::vec3 a_pos, glm::quat a_rot, const char* a_vertShader, const char* a_fragShader, Node* a_parent) :
	Node(a_pos, a_rot, a_parent), m_name(a_name), m_vertShaderFilePath(a_vertShader), m_fragShaderFilePath(a_fragShader)
{
}

RenderObject::~RenderObject()
{
}

void RenderObject::Init()
{
	CreateOpenGLBuffers();
}

void RenderObject::Update()
{
	Node::Update();
}

void RenderObject::CreateOpenGLBuffers()
{
	int success = GL_FALSE;
	unsigned int vertexShader = LoadShaderFromFile(m_vertShaderFilePath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = LoadShaderFromFile(m_fragShaderFilePath, GL_FRAGMENT_SHADER);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// Generate GL buffers
	glGenBuffers(1, &m_VBO);

	// create and bind buffers to a vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

#pragma region SubData Attempt

	int vertArraySize = sizeof(GLfloat) * m_vertexCount;
	int normalArraySize = sizeof(GLfloat) * m_normalCount;
	int texArraySize = sizeof(GLfloat) * m_texcoordCount;
	int totalArraySize = vertArraySize + normalArraySize + texArraySize;

	glBufferData(GL_ARRAY_BUFFER, totalArraySize, (GLvoid*)arrays, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); // Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)vertArraySize); // Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)(vertArraySize + normalArraySize)); // Texcoord
	check_gl_error();

#pragma endregion

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderObject::DrawElements()
{
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glUseProgram(0);
}

unsigned int RenderObject::LoadShaderFromFile(const char * _filePath, unsigned int _shaderType)
{
	//if (shaderList.find(_filePath) == shaderList.end())
	{
		FILE* file;
		fopen_s(&file, _filePath, "rb");
		if (file == nullptr)
			return 0;

		// Read the shader source
		fseek(file, 0, SEEK_END);
		unsigned int length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* source = new char[length + 1];
		memset(source, 0, length + 1);
		fread(source, sizeof(char), length, file);
		fclose(file);

		unsigned int shader = glCreateShader(_shaderType);
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);
		delete[] source;

		return shader;
	}
}