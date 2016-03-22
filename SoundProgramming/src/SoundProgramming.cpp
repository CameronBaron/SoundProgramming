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

	result = m_pFModSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &m_pEchoDSP);
	createSound(&m_pSound, "./data/audio/background_music.ogg");
	createSound(&m_gunSound, "./data/audio/gunshot.ogg");
	result = m_pFModSystem->createChannelGroup("Music Channel Group", &m_channelGroupMusic);

#pragma endregion

	// Play background Music!
	playSound(m_pSound, m_FMChannelPos, m_FMChannelVel, 0);

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

	Gizmos::clear();
    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

    m_camera.update(dt);

	m_FMposition	= { m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z };
	m_FMvelocity	= { 0, 0, 0};
	m_FMforward		= { 1, 0, 0};
	m_FMup			= { 0, 1, 0};

	m_pFModSystem->set3DListenerAttributes(0, &m_FMposition, &m_FMvelocity, &m_FMforward, &m_FMup);

	guntimer += dt;
	if (guntimer >= gunCooldown && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		//play sound m_gunSound
		playSound(m_gunSound, m_FMposition, m_FMvelocity, dt);
		guntimer = 0;
	}

	m_pFModSystem->update();

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

void SoundProgramming::createSound(FMOD::Sound** pSound, const char * pFile)
{
	result = m_pFModSystem->createSound(pFile, FMOD_CREATESTREAM | FMOD_3D, 0, pSound);
	//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

void SoundProgramming::playSound(FMOD::Sound* pSound, FMOD_VECTOR pPos, FMOD_VECTOR pVel, float dt, bool bLoop)
{
	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	FMOD::Channel* pChannel;
	result = m_pFModSystem->playSound(pSound, m_channelGroupMusic, false, &pChannel);
	//result = pChannel->addDSP(0, m_pEchoDSP);
	//result = pChannel->set3DMinMaxDistance(0.1f, 100);
	//result = pChannel->setPitch(50 * dt);
	//result = pChannel->set3DAttributes(&pPos, &pVel, 0);
	//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

void SoundProgramming::releaseSound(FMOD::Sound * pSound)
{
	pSound->release();
}

void SoundProgramming::UpdateGUI()
{

}
