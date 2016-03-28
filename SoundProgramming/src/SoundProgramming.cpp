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

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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

	FMOD_VECTOR cube[24] = //6 faces times 4 verts = 24
	{
		{ 1, -1, -1 },{ 1, -1, 1 },{ 1, 1, 1 },{ 1, 1, -1 }, //+X face
		{ -1, -1, -1 },{ -1, -1, 1 },{ -1, 1, 1 },{ -1, 1, -1 }, //-X face
		{ -1, 1, -1 },{ 1, 1, -1 },{ 1, 1, 1 },{ -1, 1, 1 }, //+Y face
		{ -1, -1, -1 },{ 1, -1, -1 },{ 1, -1, 1 },{ -1, -1, 1 }, //-Y face
		{ -1, -1, 1 },{ -1, 1, 1 },{ 1, 1, 1 },{ 1, 1, -1 }, //+Z face
		{ -1, -1, -1 },{ -1, 1, -1 },{ 1, 1, -1 },{ 1,-1, -1 }, //-Z face
	};

	m_pFModSystem->createGeometry(6, 24, &geometry);

	int pi = 0;
	for (int i = 0; i < 6; ++i)
	{
		result = geometry->addPolygon(1, 1, 1, 4, cube + (4 * i), &pi); // pointer arithmetic to get face i
	}

	CreateCommonSounds();
	result = m_pFModSystem->createChannelGroup("Music Channel Group", &m_channelGroupMusic);

	bgSound->Play();
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

#pragma region Gizmos
	Gizmos::clear();
    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10), i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i), i == 10 ? white : black);
    }
	Gizmos::addSphere(vec3((float)sinf(glfwGetTime()) * 5, 0, cos((float)glfwGetTime()) * 5), 0.1f, 5, 5, vec4(1, 0, 0, 1));
	Gizmos::addAABB(vec3(0), vec3(1, 1, 1), vec4(0, 0, 1, 1));
	
#pragma endregion
	
    m_camera.update(dt);

#pragma region FMOD
	/*
	foreach (sound in list of sounds)
		if (sound.isplaying)
			sound.update();
	*/

	m_listenerPosition = { m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z };
	m_listenerVelocity = { 0, 0, 0 };
	m_listenerForward = { m_camera.front.x, m_camera.front.y, m_camera.front.z };
	m_listenerUp = { m_camera.up.x, m_camera.up.y, m_camera.up.z };
	result = m_pFModSystem->set3DListenerAttributes
		(0, &m_listenerPosition, &m_listenerVelocity, &m_listenerForward, &m_listenerUp);


	bgSound->m_channelPosition = { sinf((float)glfwGetTime()) * 5, 0, cos((float)glfwGetTime()) * 5 };

	bgSound->Update();

	//guntimer += dt;
	//if (guntimer >= gunCooldown && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	//{
	//	//play sound m_gunSound
	//	playSound(m_gunSound, m_FMposition, m_FMvelocity, dt);
	//	guntimer = 0;
	//}

	result = m_pFModSystem->update();
#pragma endregion
	
	UpdateGUI();
	
    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplGlfwGL3_NewFrame();

    Gizmos::draw(m_camera.proj, m_camera.view);

	ImGui::Render();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

	// Calls Render for ImGui
}

void SoundProgramming::CreateCommonSounds()
{
	bgSound = new SoundClass(&m_pFModSystem, m_channelGroupMusic, m_FMChannelPos, "./data/audio/background_music.ogg");
	bgSound->m_loop = true;
}

void SoundProgramming::UpdateGUI()
{

}