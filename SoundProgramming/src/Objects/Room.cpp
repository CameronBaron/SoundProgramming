#include "Room.h"


Room::Room(vec3 a_pos, float a_scale, float a_wallOcc, float a_wallReverb, vec4 a_wallColour, float a_reverbInnerRadius, float a_reverbOuterReverb) :
	position(a_pos), scale(a_scale), wallOcclusion(a_wallOcc), wallReverb(a_wallReverb), wallColour(a_wallColour), reverbInnerRadius(a_reverbInnerRadius), reverbOuterRadius(a_reverbOuterReverb)
{
}

Room::~Room()
{
}

void Room::Init(FMOD::System* a_FMsystem)
{
	// Floor
	walls[0] = new Cube(position - vec3(0, reverbOuterRadius,0), vec3(scale, 0.1f * scale, scale), wallColour, wallOcclusion, wallReverb, true);
	// Roof
	walls[1] = new Cube(position + vec3(0, reverbOuterRadius, 0), vec3(scale, 0.1f * scale, scale), wallColour, wallOcclusion, wallReverb, true);
	// Left Wall
	walls[2] = new Cube(position - vec3(reverbOuterRadius, 0, 0), vec3(0.1f * scale, scale, scale), wallColour, wallOcclusion, wallReverb, true);
	// Right Wall
	walls[3] = new Cube(position + vec3(reverbOuterRadius, 0, 0), vec3(0.1f * scale, scale, scale), wallColour, wallOcclusion, wallReverb, true);
	// Back Wall
	walls[4] = new Cube(position - vec3(0, 0, reverbOuterRadius), vec3(scale, scale, 0.1f * scale), wallColour, wallOcclusion, wallReverb, true);

	for (int i = 0; i < 5; ++i)
	{
		a_FMsystem->createGeometry(6, 24, &walls[i]->m_geometry);
		walls[i]->Init();
	}
}

void Room::Update()
{
	for (int i = 0; i < 5; ++i)
	{
		walls[i]->Update();
	}
}

void Room::Draw()
{
}
