#pragma once
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool m_enabled;
	std::string m_name;

	//mat4 m_localTransform;
	//mat4 m_worldTransform;
};

