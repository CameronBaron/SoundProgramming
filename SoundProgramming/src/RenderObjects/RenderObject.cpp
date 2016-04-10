#include "RenderObject.h"
#include <string>
#include <iostream>

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
	unsigned int vertexShader = LoadShaderFromFile("./data/Shaders/shaders.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShader = LoadShaderFromFile("./data/Shaders/shaders.frag", GL_FRAGMENT_SHADER);

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

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate GL buffers
	glGenBuffers(1, &m_IBO);
	glGenBuffers(1, &m_VBO);

	// create and bind buffers to a vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertexCount, m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indicesCount, m_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	//glEnableVertexAttribArray(1); // Texcoord
	//glEnableVertexAttribArray(2); // Normal
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (char*)0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderObject::DrawElements()
{
	glUseProgram(m_programID);
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(0);
	//glUseProgram(0);
}

//void RenderObject::CalculateNormals()

//{
//	unsigned int arraySize = m_rows * m_columns;
//
//	for (unsigned int i = 0; i < arraySize; ++i)
//	{
//		unsigned int currentRow = i / m_rows;
//		unsigned int currentColumn = i % m_rows;
//		glm::vec3 vAverage;
//
//		std::vector<glm::vec4> vectors;
//
//		// Get surrounding vertices
//		glm::vec4 position = m_vertices[i].position;
//		// if vertZ > 0, get vertZ - 1 row
//		if (currentColumn > 0) // Left
//		{
//			vectors.push_back(m_vertices[currentRow * m_columns + (currentColumn - 1)].position - position);
//		}
//		// if vertX > 0, get vertX - 1 row
//		if (currentRow > 0) // up
//		{
//			vectors.push_back(m_vertices[(currentRow - 1) * m_columns + currentColumn].position - position);
//		}
//		// if vertz < _rows - 1, get vertZ + 1 row
//		if (currentColumn < m_columns - 1) // Right
//		{
//			vectors.push_back(m_vertices[currentRow * m_columns + (currentColumn + 1)].position - position);
//		}
//		// if vertX < _cols - 1, get vertX + 1 row
//		if (currentRow < m_rows - 1) // Down
//		{
//			vectors.push_back(m_vertices[(currentRow + 1) * m_columns + currentColumn].position - position);
//		}
//		for (unsigned int j = 0; j < vectors.size() - 1; ++j)
//		{
//			glm::vec3 vCross = -glm::cross(glm::vec3(vectors[j]), glm::vec3(vectors[j + 1]));
//			vAverage += vCross;
//		}
//
//		m_vertices[i].normal = glm::vec4(glm::normalize(vAverage), 1);
//	}
//}

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

		//std::pair<const char*, unsigned int> pairToAdd;
		//pairToAdd = std::make_pair(_filePath, shader);

		//shaderList.emplace(pairToAdd);

		return shader;
	}
	//else
	//{
	//	return shaderList.at(_filePath);
	//}
}