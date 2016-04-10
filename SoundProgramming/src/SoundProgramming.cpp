#include "SoundProgramming.h"

#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include "Gizmos.h"
#include <iostream>

bool SoundProgramming::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

#pragma region Initialize FMOD

	m_pFModSystem = nullptr;

	// Create the main system object
	result = FMOD::System_Create(&m_pFModSystem);

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	// Initialize FMOD with 512 channels
	result = m_pFModSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	SongFilePaths();

	room = new OpenBox(vec3(5), vec3(1), 0.5f, 0.5f, 0.2f, 5, songFiles[1]);
	room->Init(m_pFModSystem);

#pragma endregion
	
    return true;
}

void SoundProgramming::shutdown()
{
    Gizmos::destroy();

	m_pFModSystem->close();
	m_pFModSystem->release();

    Application::shutdown();
}

bool SoundProgramming::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    dt = (float)glfwGetTime() - lastFrameTime;
	lastFrameTime = (float)glfwGetTime();

#pragma region FMOD

	m_listenerPosition = { m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z };
	m_listenerVelocity = { 0, 0, 0 };
	m_listenerForward = { m_camera.front.x, m_camera.front.y, m_camera.front.z };
	m_listenerUp = { m_camera.up.x, m_camera.up.y, m_camera.up.z };
	result = m_pFModSystem->set3DListenerAttributes
		(0, &m_listenerPosition, &m_listenerVelocity, &m_listenerForward, &m_listenerUp);

	//guntimer += dt;
	//if (guntimer >= gunCooldown && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	//{
	//	//play sound m_gunSound
	//	playSound(m_gunSound, m_FMposition, m_FMvelocity, dt);
	//	guntimer = 0;
	//}

	result = m_pFModSystem->update();
#pragma endregion

#pragma region Gizmos
	Gizmos::clear();

    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10), i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i), i == 10 ? white : black);
    }
	
#pragma endregion

	//room->m_rotation = glm::rotate(room->m_rotation, dt, vec3(1, 0, 0));
	
	room->Update();
	
    m_camera.update(dt);	
	
    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplGlfwGL3_NewFrame();

	room->Draw(&m_camera);

    Gizmos::draw(m_camera.proj, m_camera.view);
	DrawGUI();

	ImGui::Render();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

	// Calls Render for ImGui
}

void SoundProgramming::DrawGUI()
{
	float gui_alpha = -1;
	bool gui_opened = true;

	ImGui::Begin("Main WIndow", &gui_opened, ImVec2(350, 350), gui_alpha, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	room->DrawGUI();

	ImGui::End();
}

void SoundProgramming::SongFilePaths()
{
	songFiles[0] = "./data/audio/evironmental_effect_01.ogg";
	songFiles[1] = "./data/audio/background_music.ogg";
	songFiles[2] = "./data/audio/environmental_effect_02.ogg";
	songFiles[3] = "";
	songFiles[4] = "./data/audio/AMemoryAway.ogg";

	soundProps[0] = FMOD_PRESET_UNDERWATER;
	soundProps[1] = FMOD_PRESET_CITY;
	soundProps[2] = FMOD_PRESET_HANGAR;
	soundProps[3] = FMOD_PRESET_PADDEDCELL;
	soundProps[4] = FMOD_PRESET_ALLEY;
}