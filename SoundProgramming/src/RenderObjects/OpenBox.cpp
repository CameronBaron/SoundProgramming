#include "OpenBox.h"
#include "../Camera.h"
#include "../../deps/imgui/imgui.h"

OpenBox::OpenBox(glm::vec3 a_pos, glm::vec3 a_scale, float a_wallOcc, float a_wallReverb, float a_reverbRadius, float a_reverbOuterReverb, const char* a_soundFilePath) : 
	Node(a_pos, glm::quat(), nullptr), m_wallOcclusion(a_wallOcc), m_wallReverb(a_wallReverb), m_reverbInnerRadius(a_reverbRadius), m_reverbOuterRadius(a_reverbOuterReverb), m_audioFilePath(a_soundFilePath)
{
	m_position = a_pos;
	m_scale = a_scale;
}


OpenBox::~OpenBox()
{
}

void OpenBox::Init(FMOD::System* a_FMsystem)
{
	properties = new const char*[24];
	InitReverbPresets();

	m_reverbInnerRadius *= m_scale.x;
	m_reverbOuterRadius *= m_scale.x;
	float thickness = 0.01f;
	float side = 8;

	m_walls[0] = new Rect(std::string("Left Wall"),   glm::vec3(-m_reverbOuterRadius, 0, 0), glm::vec3(thickness, side, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Left
	m_walls[1] = new Rect(std::string("Right Wall"),  glm::vec3( m_reverbOuterRadius, 0, 0), glm::vec3(thickness, side, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Right
	m_walls[2] = new Rect(std::string("Top Wall"),	  glm::vec3(0,  m_reverbOuterRadius, 0), glm::vec3(side, thickness, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Top
	m_walls[3] = new Rect(std::string("Bottom Wall"), glm::vec3(0, -m_reverbOuterRadius, 0), glm::vec3(side, thickness, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Back
	m_walls[4] = new Rect(std::string("Back Wall"),   glm::vec3(0, 0, -m_reverbOuterRadius), glm::vec3(side, side, thickness), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Bottom

	for (int i = 0; i < 5; ++i)
	{
		a_FMsystem->createGeometry(6, 24, &m_walls[i]->m_geometry);
		m_walls[i]->Init();
	}

	a_FMsystem->createChannelGroup("Music Channel Group", &m_audioGroup);

	m_sound = new SoundClass(&a_FMsystem, m_audioGroup, m_soundPosition, m_audioFilePath);
	m_sound->m_loop = true;
	m_sound->Play();
	FMOD_VECTOR pos = { m_position.x, m_position.y, m_position.z };
	m_sound->m_channelPosition = pos;
	m_sound->m_channelRef->setVolume(m_volume);

	m_reverb->set3DAttributes(&m_reverbPosition, m_reverbInnerRadius, m_reverbOuterRadius);
	m_reverb->setProperties(&m_reverbProps);
}

void OpenBox::Update()
{
	Node::Update();

	FMOD_VECTOR pos = { m_position.x, m_position.y, m_position.z };
	m_sound->m_channelPosition = pos;
	m_sound->m_channelRef->setVolume(m_volume);

	m_reverb->setProperties(&m_reverbProps);
	m_sound->Update();
	
	//Gizmos::addSphere(m_position, m_reverbOuterRadius, 20, 20, glm::vec4(0), &m_localMatrix);
}

void OpenBox::Draw(Camera* a_camera)
{
	for (int i = 0; i < 5; i++)
	{
		glUseProgram(m_walls[i]->m_programID);
		unsigned int projectionViewUniform = glGetUniformLocation(m_walls[i]->m_programID, "MVP");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_camera->view_proj * m_walls[i]->m_worldMatrix));
	
		m_walls[i]->DrawElements();
	}
	glUseProgram(0);
}

void OpenBox::DrawGUI()
{
	if (ImGui::CollapsingHeader("Room 1"))
	{
		ImGui::DragFloat3("Position", &m_position[0]);
	}
	if (ImGui::CollapsingHeader("Reverb Zone"))
	{
		ImGui::DragFloat("Inner Radius", &m_reverbInnerRadius, 1, 0, m_reverbOuterRadius);
		ImGui::DragFloat("Outer Radius", &m_reverbOuterRadius, 1, m_reverbInnerRadius);
		//ImGui::ListBox("Reverb Properties", 0, properties, 25);
	}
	if (ImGui::CollapsingHeader("Sound Source"))
	{
		ImGui::DragFloat("Volume", &m_volume, 0.1f, 0.0f, 30.0f);
		ImGui::Checkbox("Disable Reverb", &m_sound->dsp_reverbBypass);
		ImGui::Checkbox("Disable Chorus", &m_sound->dsp_chorusBypass);
	}

	// ImGui::End already called in SounProgramming.ccp
}

void OpenBox::InitReverbPresets()
{
	properties [0] = "Generic";
	properties [1] = "Generic";
	properties [2] = "Generic";
	properties [3] = "Generic";
	properties [4] = "Generic";
	properties [5] = "Generic";
	properties [6] = "Generic";
	properties [7] = "Generic";
	properties [8] = "Generic";
	properties [9] = "Generic";
	properties[10] = "Generic";
	properties[11] = "Generic";
	properties[12] = "Generic";
	properties[13] = "Generic";
	properties[14] = "Generic";
	properties[15] = "Generic";
	properties[16] = "Generic";
	properties[17] = "Generic";
	properties[18] = "Generic";
	properties[19] = "Generic";
	properties[20] = "Generic";
	properties[21] = "Generic";
	properties[22] = "Generic";
	properties[23] = "Generic";
	properties[24] = "Generic";
}