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
	delete[] properties;
	delete[] reverbs;
}

void OpenBox::Init(FMOD::System* a_FMsystem)
{
	properties = new const char*[24];
	reverbs = new FMOD_REVERB_PROPERTIES[24];

	float thickness = 0.01f;
	float side = 4;

	m_walls[0] = new Rect(std::string("Left Wall"),   glm::vec3(-m_reverbInnerRadius, 0, 0), glm::vec3(thickness, side, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Left
	m_walls[1] = new Rect(std::string("Right Wall"),  glm::vec3(m_reverbInnerRadius, 0, 0),  glm::vec3(thickness, side, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Right
	m_walls[2] = new Rect(std::string("Top Wall"),	  glm::vec3(0, m_reverbInnerRadius, 0),  glm::vec3(side, thickness, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Top
	m_walls[3] = new Rect(std::string("Bottom Wall"), glm::vec3(0, -m_reverbInnerRadius, 0), glm::vec3(side, thickness, side), glm::quat(), m_cubeVertShader, m_cubeFragShader, m_wallOcclusion, m_wallReverb, this); // Bottom
	// Display audio visualisation on this one, will need custom shaders
	m_walls[4] = new Rect(std::string("Back Wall"),   glm::vec3(0, 0, -m_reverbInnerRadius), glm::vec3(side, side, thickness), glm::quat(), "./data/Shaders/eq.vert", "./data/Shaders/eq.frag", m_wallOcclusion, m_wallReverb, this); // Back

	for (int i = 0; i < 4; ++i)
	{
		a_FMsystem->createGeometry(6, 24, &m_walls[i]->m_geometry);
		m_walls[i]->Init();
	}
	a_FMsystem->createGeometry(6, 24, &m_walls[4]->m_geometry);
	m_walls[4]->Init();
	m_walls[4]->LoadTextureData(m_sampleSize, equaliserValues);
	// Texture image stuff

	a_FMsystem->createChannelGroup("Music Channel Group", &m_audioGroup);

	m_sound = new SoundClass(&a_FMsystem, m_audioGroup, m_soundPosition, m_audioFilePath);
	m_sound->m_loop = true;
	m_sound->Play();
	FMOD_VECTOR pos = { m_position.x, m_position.y, m_position.z };
	m_sound->m_channelPosition = pos;
	m_sound->m_channelRef->setVolume(m_volume);	

	a_FMsystem->createReverb3D(&m_reverb);
	m_reverb->set3DAttributes(&pos, m_reverbInnerRadius, m_reverbOuterRadius);
	m_reverb->setProperties(&m_reverbProps);
	InitReverbPresets();

	//Lighting
	LoadLightUniformLocs();
	lightColor = vec3(1, 1, 1);
	lightAmbient = 20;
	lightConstantAtt = 1;
	lightExpAtt = 1;
	lightLinearAtt = 1;
}

void OpenBox::Update()
{
	m_rotation = glm::quat(1,20,1,1);
	Node::Update();

	m_rotation = glm::quat(1, 1, 1, 1);

	FMOD_VECTOR pos = { m_position.x, m_position.y, m_position.z };
	m_sound->m_channelPosition = pos;
	m_sound->m_channelRef->setVolume(m_volume);
	
	m_reverb->set3DAttributes(&pos, m_reverbInnerRadius, m_reverbOuterRadius);
	m_reverb->setProperties(&m_reverbProps);
	m_sound->Update();

	equaliserValues = &m_sound->fftHeights[0];
	
	m_walls[4]->UpdateTexData(m_sampleSize, equaliserValues);

	lightColor = vec3(rand() % 2, rand() % 2, rand() % 2);

	//Lighting
}

void OpenBox::Draw(Camera* a_camera)
{
	for (int i = 0; i < 5; i++)
	{
		unsigned int currProg = m_walls[i]->m_programID;
		glUseProgram(currProg);
		unsigned int projectionViewUniform = glGetUniformLocation(currProg, "MVP");
		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(a_camera->view_proj * m_walls[i]->m_worldMatrix));

		// Uniform for beat value to use for lighting
		unsigned int beatUniform = glGetUniformLocation(currProg, "beatIntensity");
		glUniform1f(beatUniform, m_sound->beatIntensity);

		//worldPos
		glUniform3fv(m_walls[i]->worldPosLoc, 1, glm::value_ptr(m_walls[i]->m_worldMatrix[3]));
		//light.vColor
		glUniform3fv(m_walls[i]->lightColorLoc, 1, glm::value_ptr(lightColor));
		//light.vPosition
		glUniform3fv(m_walls[i]->lightPosLoc, 1, glm::value_ptr(m_position));
		//light.fAmbient
		glUniform1f(m_walls[i]->lightAmbientLoc, lightAmbient);
		//light.fConstantAtt
		glUniform1f(m_walls[i]->lightConstantAttLoc, lightConstantAtt);
		//light.fLinearAtt
		glUniform1f(m_walls[i]->lightLinearAttLoc, lightLinearAtt);
		//light.fExpAtt
		glUniform1f(m_walls[i]->lightExpAttLoc, lightExpAtt);

		if (i == 4)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, m_walls[i]->m_equaliserTex);
		}
	
		m_walls[i]->DrawElements();
	}
	glUseProgram(0);
}

void OpenBox::DrawGUI()
{
	std::string name = "Room" + std::to_string(roomID);
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		ImGui::DragFloat3("Position", &m_position[0]);
		if (ImGui::CollapsingHeader("Reverb Zone"))
		{
			ImGui::DragFloat("Inner Radius", &m_reverbInnerRadius, 1, 0.0f, m_reverbOuterRadius);
			ImGui::DragFloat("Outer Radius", &m_reverbOuterRadius, 1, m_reverbInnerRadius, 50.0f);

			// Reverb properties list box
			if (ImGui::ListBox("Reverb Properties", &currProp, properties, 24))
			{
				m_reverbProps = reverbs[currProp];
			}
		}
		ImGui::TextColored(ImVec4(0,1,1,1),"Sound Source");
		{
			ImGui::DragFloat("Volume", &m_volume, 0.01f, 0.0f, 30.0f);
			ImGui::Checkbox("Disable Reverb", &m_sound->dsp_reverbBypass);
		}
	}
	// ImGui::End already called in SoundProgramming.ccp
}

void OpenBox::InitReverbPresets()
{
	properties [0] = "Off";
	properties [1] = "Generic";
	properties [2] = "Padded Cell";
	properties [3] = "Room";
	properties [4] = "Bathroom";
	properties [5] = "LivingRoom";
	properties [6] = "Stoneroom";
	properties [7] = "Auditorium";
	properties [8] = "Concert Hall";
	properties [9] = "Cave";
	properties[10] = "Arena";
	properties[11] = "Hanger";
	properties[12] = "Carpetted Hallway";
	properties[13] = "Hallway";
	properties[14] = "Stone Corridor";
	properties[15] = "Alley";
	properties[16] = "Forest";
	properties[17] = "City";
	properties[18] = "Mountains";
	properties[19] = "Quarry";
	properties[20] = "Plain";
	properties[21] = "Parking Lot";
	properties[22] = "Sewer Pipe";
	properties[23] = "Underwater";

	reverbs [0] = FMOD_PRESET_OFF;
	reverbs [1] = FMOD_PRESET_GENERIC;
	reverbs [2] = FMOD_PRESET_PADDEDCELL;
	reverbs [3] = FMOD_PRESET_ROOM;
	reverbs [4] = FMOD_PRESET_BATHROOM;
	reverbs [5] = FMOD_PRESET_LIVINGROOM;
	reverbs [6] = FMOD_PRESET_STONEROOM;
	reverbs [7] = FMOD_PRESET_AUDITORIUM;
	reverbs [8] = FMOD_PRESET_CONCERTHALL;
	reverbs [9] = FMOD_PRESET_CAVE;
	reverbs[10] = FMOD_PRESET_ARENA;
	reverbs[11] = FMOD_PRESET_HANGAR;
	reverbs[12] = FMOD_PRESET_CARPETTEDHALLWAY;
	reverbs[13] = FMOD_PRESET_HALLWAY;
	reverbs[14] = FMOD_PRESET_STONECORRIDOR;
	reverbs[15] = FMOD_PRESET_ALLEY;
	reverbs[16] = FMOD_PRESET_FOREST;
	reverbs[17] = FMOD_PRESET_CITY;
	reverbs[18] = FMOD_PRESET_MOUNTAINS;
	reverbs[19] = FMOD_PRESET_QUARRY;
	reverbs[20] = FMOD_PRESET_PLAIN;
	reverbs[21] = FMOD_PRESET_PARKINGLOT;
	reverbs[22] = FMOD_PRESET_SEWERPIPE;
	reverbs[23] = FMOD_PRESET_UNDERWATER;
}

void OpenBox::LoadLightUniformLocs()
{
	for (int i = 0; i < 5; i++)
	{
		unsigned int currProg = m_walls[i]->m_programID;
		glUseProgram(currProg);

		//worldPos
		m_walls[i]->worldPosLoc			= glGetUniformLocation(currProg, "worldPos");
		//light.vColor
		m_walls[i]->lightColorLoc		= glGetUniformLocation(currProg, "light.Color");
		//light.vPosition
		m_walls[i]->lightPosLoc			= glGetUniformLocation(currProg, "light.Position");
		//light.fAmbient
		m_walls[i]->lightAmbientLoc		= glGetUniformLocation(currProg, "light.fAmbient");
		//light.fConstantAtt
		m_walls[i]->lightConstantAttLoc = glGetUniformLocation(currProg, "light.fConstantAtt");
		//light.fLinearAtt
		m_walls[i]->lightLinearAttLoc	= glGetUniformLocation(currProg, "light.fLinearAtt");
		//light.fExpAtt
		m_walls[i]->lightExpAttLoc		= glGetUniformLocation(currProg, "light.fExpAtt");
	}
	glUseProgram(0);
}