#include "Room.h"
#include <math.h>

#define PI 3.14f

Room::Room(vec3 a_pos, vec3 a_scale, float a_wallOcc, float a_wallReverb, vec4 a_wallColour, float a_reverbInnerRadius, float a_reverbOuterReverb) : 
	BaseObject(a_pos, a_scale, vec4(1)), 
	wallOcclusion(a_wallOcc), wallReverb(a_wallReverb), wallColour(a_wallColour), reverbInnerRadius(a_reverbInnerRadius), reverbOuterRadius(a_reverbOuterReverb)
{
}

Room::~Room()
{
}

void Room::Init(FMOD::System* a_FMsystem)
{
	// Floor
	walls[0] = new Cube(m_position - vec3(0, reverbOuterRadius, 0), vec3(m_scale.x, 0.1f * m_scale.y, m_scale.z), vec4(1, 0.2f, 0, 1), wallOcclusion, wallReverb, true, this);
	// Roof																									 
	walls[1] = new Cube(m_position + vec3(0, reverbOuterRadius, 0), vec3(m_scale.x, 0.1f * m_scale.y, m_scale.z), vec4(0, 1, 0, 1), wallOcclusion, wallReverb, true, this);
	// Left Wall																							 
	walls[2] = new Cube(m_position - vec3(reverbOuterRadius, 0, 0), vec3(0.1f * m_scale.x, m_scale.y, m_scale.z), vec4(1, 0, 0, 1), wallOcclusion, wallReverb, true, this);
	// Right Wall																							 
	walls[3] = new Cube(m_position + vec3(reverbOuterRadius, 0, 0), vec3(0.1f * m_scale.x, m_scale.y, m_scale.z), vec4(1, 0, 0, 1), wallOcclusion, wallReverb, true, this);
	// Back Wall																							 
	walls[4] = new Cube(m_position - vec3(0, 0, reverbOuterRadius), vec3(m_scale.x, m_scale.y, 0.1f * m_scale.z), vec4(1, 0, 0.2f, 1), wallOcclusion, wallReverb, true, this);

	for (int i = 0; i < 5; ++i)
	{
		a_FMsystem->createGeometry(6, 24, &walls[i]->m_geometry);
		walls[i]->Init();
	}

	a_FMsystem->createChannelGroup("Music Channel Group", &audioGroup);

	sound = new SoundClass(&a_FMsystem, audioGroup, soundPosition, soundFilePath);
	sound->m_loop = true;
	sound->Play();
	FMOD_VECTOR pos = { m_position.x + sound->m_channelPosition.x, m_position.y + sound->m_channelPosition.y, m_position.z + sound->m_channelPosition.z };
	sound->m_channelPosition = pos;
	sound->m_channelRef->setVolume(20);

	reverb->set3DAttributes(&reverbPosition, 10, 20);
	reverb->setProperties(&reverbProps);
}

void Room::Update()
{
	sound->Update();

	//for (int i = 0; i < 5; ++i)
	//{
	//	walls[i]->Update();
	//}
	BaseObject::Update();

	Gizmos::addSphere(m_position, reverbInnerRadius, 10, 10, vec4(0, 0, 1, 0.5f));
	Gizmos::addSphere(m_position, reverbOuterRadius, 25, 25, vec4(0, 0, 1, 0.5f));
}

void Room::Draw()
{
}