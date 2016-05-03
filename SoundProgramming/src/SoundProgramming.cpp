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

    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glEnable(GL_DEPTH_TEST);
	window_width = 1280;
	window_height = 780;
    m_camera = FlyCamera(1280.0f / 780.0f, 10.0f);
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
		rooms[i] = new OpenBox(vec3(-40 + (i * 20), 0, 0), vec3(1), 0.5f, 1, 3, 7, songFiles[i]);
		rooms[i]->roomID = i + 1;
		rooms[i]->Init(m_pFModSystem);
	}

#pragma endregion
	
    return true;
}

void SoundProgramming::shutdown()
{
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

	for (OpenBox* ob : rooms)
	{
		ob->Update();
	}
	
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	if (w != window_width || h != window_height)
	{
		glViewport(0, 0, w, h);
		window_width = w;
		window_height = h;
	}
    m_camera.update(dt);	
	
    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplGlfwGL3_NewFrame();

	for (OpenBox* ob : rooms)
	{
		ob->Draw(&m_camera);
	}

	DrawGUI();

	ImGui::Render();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void SoundProgramming::DrawGUI()
{
	float gui_alpha = -1;
	bool gui_opened = true;

	ImGui::Begin("Main WIndow", &gui_opened, ImVec2(350, 500), gui_alpha, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	if (ImGui::CollapsingHeader("Listener Options"))
	{
		ImGui::DragFloat("Doppler Level", &dopplerLevel, 0.01f, 0.0f, 1.0f);
	}

	for (OpenBox* ob : rooms)
	{
		ob->DrawGUI();
	}

	ImGui::End();
}

void SoundProgramming::SongFilePaths()
{
	songFiles[2] = "./data/audio/Ratatat/02 Cream on Chrome.mp3";
	songFiles[1] = "./data/audio/Ratatat/Holdin' On.mp3";
	songFiles[0] = "./data/audio/Ratatat/04 Abrasive.mp3";
	songFiles[3] = "./data/audio/Ratatat/01 Sintra 1.mp3";
	songFiles[4] = "./data/audio/Ratatat/08 Nightclub Amnesia.mp3";

	//songFiles[0] = "./data/audio/Ratatat/02 Cream on Chrome.mp3";
	//songFiles[1] = "./data/audio/background_music.ogg";
	//songFiles[2] = "./data/audio/GetSchwifty.mp3";
	//songFiles[3] = "./data/audio/SeinfeldTheme.mp3";
	//songFiles[4] = "./data/audio/Rain.mp3";

	soundProps[0] = FMOD_PRESET_OFF;
	soundProps[1] = FMOD_PRESET_OFF;
	soundProps[2] = FMOD_PRESET_OFF;
	soundProps[3] = FMOD_PRESET_OFF;
	soundProps[4] = FMOD_PRESET_OFF;
}
