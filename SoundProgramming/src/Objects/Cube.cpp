#include "Cube.h"



Cube::Cube(vec3 pos, vec3 sc, float occ, float rev, bool filled) : BaseObject(pos, sc, occ, rev)
{
	isFilled = filled;
}


Cube::~Cube()
{
}

void Cube::Init()
{
	CreateVerts();
	
}

void Cube::Update()
{
	if (isFilled)
		Gizmos::addAABBFilled(position, scale, color, &modelMatrix);
	else
		Gizmos::addAABB(position, scale, color, &modelMatrix);
	
	// Update geometry position here
	tempPos.x = position.x;
	tempPos.y = position.y;
	tempPos.z = position.z;

	BaseObject::Update();
}

void Cube::CreateVerts()
{
	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
	{
		{  scale.x, -scale.y, -scale.z },{  scale.x, -scale.y,  scale.z },{  scale.x,  scale.y,  scale.z },{  scale.x,  scale.y, -scale.z }, //+X face
		{ -scale.x, -scale.y, -scale.z },{ -scale.x, -scale.y,  scale.z },{ -scale.x,  scale.y,  scale.z },{ -scale.x,  scale.y, -scale.z }, //-X face
		{ -scale.x,  scale.y, -scale.z },{  scale.x,  scale.y, -scale.z },{  scale.x,  scale.y,  scale.z },{ -scale.x,  scale.y,  scale.z }, //+Y face
		{ -scale.x, -scale.y, -scale.z },{  scale.x, -scale.y, -scale.z },{  scale.x, -scale.y,  scale.z },{ -scale.x, -scale.y,  scale.z }, //-Y face
		{ -scale.x, -scale.y,  scale.z },{ -scale.x,  scale.y,  scale.z },{  scale.x,  scale.y,  scale.z },{  scale.x,  scale.y, -scale.z }, //+Z face
		{ -scale.x, -scale.y, -scale.z },{ -scale.x,  scale.y, -scale.z },{  scale.x,  scale.y, -scale.z },{  scale.x, -scale.y, -scale.z }, //-Z face
	};

	int pi = 0;
	for (int i = 0; i < 6; ++i)
	{
		result = geometry->addPolygon(directOcclusion, reverbOcclusion, 1, 4, cube + (4 * i), &pi); // pointer arithmetic to get face i
	}
	geometry->setPosition(&tempPos);
}
