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


	createSound(&m_pSound, "./data/audio/background_music.ogg");
	result = m_pFModSystem->createChannelGroup("MyChannelGroup", &m_pChannelGroup);

#pragma endregion

	// Play background Music!
	playSound(m_pSound, true);

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

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    glfwSetTime(0.0);

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
	m_pChannel->set3DAttributes(&m_FMChannelPos, &m_FMChannelVel, 0);


	m_pFModSystem->update();

    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gizmos::draw(m_camera.proj, m_camera.view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void SoundProgramming::createSound(FMOD::Sound** pSound, const char * pFile)
{
	result = m_pFModSystem->createSound(pFile, FMOD_CREATESTREAM | FMOD_3D, 0, pSound);
	printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

void SoundProgramming::playSound(FMOD::Sound* pSound, bool bLoop)
{
	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}
	FMOD::Channel* pChannel;
	result = m_pFModSystem->playSound(pSound, m_pChannelGroup, false, &pChannel);
	printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
}

void SoundProgramming::releaseSound(FMOD::Sound * pSound)
{
	pSound->release();
}
