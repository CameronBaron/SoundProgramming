#include "Rect.h"
#include "../Gizmos.h"


Rect::Rect(
	std::string a_name, glm::vec3 a_pos, glm::vec3 a_size, glm::quat a_rot,
	const char* a_vertShader, const char* a_fragShader, float a_directOcclusion, float a_reverbOcclusion, Node* a_parent) :
	RenderObject(a_name, a_pos, a_rot, a_vertShader, a_fragShader, a_parent), 
	m_directOcclusion(a_directOcclusion), m_reverbOcclusion(a_reverbOcclusion)
{
	m_scale = a_size;
}


Rect::~Rect()
{
}

void Rect::Init()
{
	LoadRawData();

	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
	{
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f, -0.5f }, //+X face
		
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f, -0.5f }, //-X face
		
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f }, //+Y face
		
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f }, //-Y face
		
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f }, //+Z face
		
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f }, //-Z face
	};

	int pi = 0;
	for (int i = 0; i < 6; ++i)
	{
		result = m_geometry->addPolygon(m_directOcclusion, m_reverbOcclusion, 1, 4, cube + (4 * i), &pi); // pointer arithmetic to get face i
	}

	RenderObject::Init();
}

void Rect::Update()
{
	RenderObject::Update();

	// Update FMOD geometry position and scale incase it has changed
	FMOD_VECTOR tempPos = { m_worldMatrix[3].x, m_worldMatrix[3].y, m_worldMatrix[3].z };
	m_geometry->setPosition(&tempPos);

	FMOD_VECTOR fscale = { m_scale.x, m_scale.y, m_scale.z };
	m_geometry->setScale(&fscale);

	m_geometry->setRotation(&m_geoForward, &m_geoUp);

	// Update forward and up vectors to use with FMOD rotation
	glm::vec3 front = glm::vec3(m_worldMatrix[2].x, m_worldMatrix[2].y, m_worldMatrix[2].z);
	front = glm::normalize(front);
	m_geoForward = { front.x, front.y, front.z };
	glm::vec3 right = glm::vec3( m_worldMatrix[0].x, m_worldMatrix[0].y, m_worldMatrix[0].z );
	glm::vec3 up = glm::cross(front, right);
	up = glm::normalize(up);
	m_geoUp = { up.x, up.y, up.z };
}

void Rect::LoadRawData()
{
	glm::vec3 vertices[8] =
	{
		glm::vec3(-0.5f , -0.5f , -0.5f ),//0
		glm::vec3( 0.5f , -0.5f , -0.5f ),//1
		glm::vec3( 0.5f ,  0.5f , -0.5f ),//2
		glm::vec3(-0.5f ,  0.5f , -0.5f ),//3
		glm::vec3(-0.5f , -0.5f ,  0.5f ),//4
		glm::vec3( 0.5f , -0.5f ,  0.5f ),//5
		glm::vec3( 0.5f ,  0.5f ,  0.5f ),//6
		glm::vec3(-0.5f ,  0.5f ,  0.5f )//7
	};

	glm::vec2 texCoords[4] =
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	glm::vec3 normals[6] =
	{
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(1, 0, 0)
	};

	int indices[6 * 6] =
	{
		4,5,7,7,5,6,
		1,0,2,2,0,3,
		7,6,3,3,6,2,
		5,4,1,1,4,0,
		0,4,3,3,4,7,
		5,1,6,6,1,2
	};

	int texInds[6] = { 0, 1, 3, 3, 1, 2 };

	m_vertexCount = 18 * 6;
	m_vertBuffer = new float[m_vertexCount];
	for (size_t i = 0; i < 36; i++)
	{
		m_vertBuffer[i * 3 + 0] = vertices[indices[i]].x;
		m_vertBuffer[i * 3 + 1] = vertices[indices[i]].y;
		m_vertBuffer[i * 3 + 2] = vertices[indices[i]].z;
	}

	m_texcoordCount = 12 * 6;
	m_texcoordBuffer = new float[m_texcoordCount];
	for (size_t i = 0; i < 36; i++)
	{
		m_texcoordBuffer[i * 2 + 0] = texCoords[texInds[i % 6]].x;
		m_texcoordBuffer[i * 2 + 1] = texCoords[texInds[i % 6]].y;
	}

	m_normalCount = 18 * 6;
	m_normalBuffer = new float[m_normalCount];
	for (size_t i = 0; i < 36; i++)
	{
		m_normalBuffer[i * 3 + 0] = normals[i / 6].x;
		m_normalBuffer[i * 3 + 1] = normals[i / 6].y;
		m_normalBuffer[i * 3 + 2] = normals[i / 6].z;
	}

	arraySize = m_vertexCount + m_texcoordCount + m_normalCount;
	arrays = new float[arraySize];
	
	for (size_t i = 0; i < m_vertexCount; i++)
	{
		arrays[i] = m_vertBuffer[i];
	}
	for (size_t i = 0; i < m_normalCount; i++)
	{
		arrays[i + m_vertexCount] = m_normalBuffer[i];
	}
	for (size_t i = 0; i < m_texcoordCount; i++)
	{
		arrays[i + m_normalCount + m_vertexCount] = m_texcoordBuffer[i];
	}
}

void Rect::LoadTextureData(int a_dataSize, float* a_data)
{
	glUseProgram(m_programID);
	glGenTextures(1, &m_equaliserTex);
	glBindTexture(GL_TEXTURE_1D, m_equaliserTex);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, a_dataSize, 0, GL_RED, GL_FLOAT, a_data);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Nearest for no interpolation between pixels
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texLoc = glGetUniformLocation(m_programID, "tex");
	glUniform1i(texLoc, 0);
}

void Rect::UpdateTexData(int a_dataSize, float* a_data)
{	
	glUseProgram(m_programID);
	//uniform

	glBindTexture(GL_TEXTURE_1D, m_equaliserTex);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, a_dataSize, GL_RED, GL_FLOAT, a_data);

	glUseProgram(0);
	
}