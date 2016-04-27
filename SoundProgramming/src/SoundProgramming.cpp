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

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(0, 0, 50), vec3(0), vec3(0, 1, 0));
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

	for (int i = 0; i < 5; i++)
	{
		rooms[i] = new OpenBox(vec3(-50 + (i * 20), 0, 0), vec3(1), 0.5f, 1, 3, 7, songFiles[i]);
		rooms[i]->roomID = i + 1;
		rooms[i]->Init(m_pFModSystem);
	}

	//room = new OpenBox(vec3(0, 0, 0), vec3(1), 0.5f, 1, 3, 7, songFiles[2]);
	//room->Init(m_pFModSystem);

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

#pragma region FMOD Listener

	m_listenerPosition = { m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z };
	m_listenerVelocity = { m_camera.m_FMvelocity.x * dopplerLevel , m_camera.m_FMvelocity.y * dopplerLevel, m_camera.m_FMvelocity.z * dopplerLevel };
	m_listenerForward = { m_camera.front.x, m_camera.front.y, m_camera.front.z };
	m_listenerUp = { m_camera.up.x, m_camera.up.y, m_camera.up.z };
	result = m_pFModSystem->set3DListenerAttributes
		(0, &m_listenerPosition, &m_listenerVelocity, &m_listenerForward, &m_listenerUp);

	result = m_pFModSystem->update();
#pragma endregion

#pragma region Gizmos
	//Gizmos::clear();
	//
    //vec4 white(1);
    //vec4 black(0, 0, 0, 1);
    //for (int i = 0; i <= 20; ++i)
    //{
    //    Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10), i == 10 ? white : black);
    //    Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i), i == 10 ? white : black);
    //}
	
#pragma endregion

	//room->m_rotation = glm::rotate(room->m_rotation, dt, vec3(1, 0, 0));
	for each (OpenBox* ob in rooms)
	{
		ob->Update();
	}
	//room->Update();
	
    m_camera.update(dt);	
	
    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplGlfwGL3_NewFrame();

	for each (OpenBox* ob in rooms)
	{
		ob->Draw(&m_camera);
	}
	
	//room->Draw(&m_camera);

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
	if (ImGui::CollapsingHeader("Listener Options"))
	{
		ImGui::DragFloat("Doppler Level", &dopplerLevel, 0.01f, 0.0f, 1.0f);
	}
	//room->DrawGUI();
	for each (OpenBox* ob in rooms)
	{
		ob->DrawGUI();
	}

	ImGui::End();
}

void SoundProgramming::SongFilePaths()
{
	songFiles[0] = "./data/audio/evironmental_effect_01.ogg";
	songFiles[1] = "./data/audio/background_music.ogg";
	songFiles[2] = "./data/audio/GetSchwifty.mp3";
	songFiles[3] = "./data/audio/SeinfeldTheme.mp3";
	songFiles[4] = "./data/audio/Rain.mp3";

	soundProps[0] = FMOD_PRESET_OFF;
	soundProps[1] = FMOD_PRESET_OFF;
	soundProps[2] = FMOD_PRESET_OFF;
	soundProps[3] = FMOD_PRESET_OFF;
	soundProps[4] = FMOD_PRESET_OFF;
}