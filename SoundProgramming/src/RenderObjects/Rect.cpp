#include "Rect.h"
#include "../Gizmos.h"


Rect::Rect(
	std::string a_name, glm::vec3 a_pos, glm::vec3 a_size, glm::quat a_rot,
	const char* a_vertShader, const char* a_fragShader, float a_directOcclusion, float a_reverbOcclusion, Node* a_parent) :
	RenderObject(a_name, a_pos, a_rot, a_vertShader, a_fragShader, a_parent), 
	m_directOcclusion(a_directOcclusion), m_reverbOcclusion(a_reverbOcclusion), m_size(a_size)
{

}


Rect::~Rect()
{
}

void Rect::Init()
{
	m_vertexCount = 24;
	m_indicesCount = 36;

	m_vertices = new float[m_vertexCount]
	{
		-.5f * m_size.x, -.5f * m_size.y,  .5f * m_size.z,
		-.5f * m_size.x,  .5f * m_size.y,  .5f * m_size.z,
		 .5f * m_size.x,  .5f * m_size.y,  .5f * m_size.z,
		 .5f * m_size.x, -.5f * m_size.y,  .5f * m_size.z,
		-.5f * m_size.x, -.5f * m_size.y, -.5f * m_size.z,
		-.5f * m_size.x,  .5f * m_size.y, -.5f * m_size.z,
		 .5f * m_size.x,  .5f * m_size.y, -.5f * m_size.z,
		 .5f * m_size.x, -.5f * m_size.y, -.5f * m_size.z
	}; 

	m_indices = new unsigned int[m_indicesCount]
	{
		0, 2, 1, 0, 3, 2,
		4, 3, 0, 4, 7, 3,
		4, 1, 5, 4, 0, 1,
		3, 6, 2, 3, 7, 6,
		1, 6, 5, 1, 2, 6,
		7, 5, 6, 7, 4, 5
	};

	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
	{
		{  0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z }, //+X face
		{ -0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z },{ -0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z },{ -0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z },{ -0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z }, //-X face
		{ -0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z },{ -0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z }, //+Y face
		{ -0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z },{ -0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z }, //-Y face
		{ -0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z },{ -0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y,  0.5f * m_size.z },{  0.5f * m_size.x, -0.5f * m_size.y,  0.5f * m_size.z }, //+Z face
		{ -0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z },{ -0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x,  0.5f * m_size.y, -0.5f * m_size.z },{  0.5f * m_size.x, -0.5f * m_size.y, -0.5f * m_size.z }, //-Z face
		
		//{  0.5f, -0.5f, -0.5f },
		//{  0.5f, -0.5f,  0.5f },
		//{  0.5f,  0.5f,  0.5f },
		//{  0.5f,  0.5f, -0.5f }, //+X face
		//
		//{ -0.5f, -0.5f, -0.5f },
		//{ -0.5f, -0.5f,  0.5f },
		//{ -0.5f,  0.5f,  0.5f },
		//{ -0.5f,  0.5f, -0.5f }, //-X face
		//
		//{ -0.5f,  0.5f, -0.5f },
		//{  0.5f,  0.5f, -0.5f },
		//{  0.5f,  0.5f,  0.5f },
		//{ -0.5f,  0.5f,  0.5f }, //+Y face
		//
		//{ -0.5f, -0.5f, -0.5f },
		//{  0.5f, -0.5f, -0.5f },
		//{  0.5f, -0.5f,  0.5f },
		//{ -0.5f, -0.5f,  0.5f }, //-Y face
		//
		//{ -0.5f, -0.5f,  0.5f },
		//{ -0.5f,  0.5f,  0.5f },
		//{  0.5f,  0.5f,  0.5f },
		//{  0.5f, -0.5f,  0.5f }, //+Z face
		//
		//{ -0.5f, -0.5f, -0.5f },
		//{ -0.5f,  0.5f, -0.5f },
		//{  0.5f,  0.5f, -0.5f },
		//{  0.5f, -0.5f, -0.5f }, //-Z face
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

	// Add bounding box for visual guide
	Gizmos::addAABB(glm::vec3(m_worldMatrix[3].x, m_worldMatrix[3].y, m_worldMatrix[3].z), m_size * 0.5f, glm::vec4( 1, 0, 0, 0), &m_worldMatrix);

	// Update forward and up vectors to use with FMOD rotation
	glm::vec3 front = glm::vec3(m_worldMatrix[2].x, m_worldMatrix[2].y, m_worldMatrix[2].z);
	m_geoForward = { front.x, front.y, front.z };
	glm::vec3 right = glm::vec3( m_worldMatrix[0].x, m_worldMatrix[0].y, m_worldMatrix[0].z );
	glm::vec3 up = glm::cross(front, right);
	m_geoUp = { up.x, up.y, up.z };
}


