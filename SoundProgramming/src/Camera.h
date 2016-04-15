#ifndef CAMERA_H_
#define CAMERA_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:
	Camera(){}
	Camera(float aspect);

	void updateViewProj();
	void setPosition(vec3 pos);
	const vec3 getPosition() { return vec3(world[3].x, world[3].y, world[3].z); }
	void setPerspective(float fov, float aspect, float near, float far);
	void setLookAt(vec3 pos, vec3 center, vec3 up);

	virtual void update(float dt) = 0;

    vec3 pickAgainstPlane(float x, float y, vec4 plane);

	mat4 world;
	mat4 view;
	mat4 proj;
	mat4 view_proj;
};

class FlyCamera : public Camera
{
public:
	FlyCamera(){}
	FlyCamera(float aspect, float speed);
	virtual void update(float dt);

	void UpdateVectors();
	void GetVelocity();

	float m_speed;
	float sensitivity;

	float yaw;
	float pitch;

	bool m_clicked_down;

	vec3 position, front, up, right, worldUp;
	vec3 m_FMvelocity;
	vec3 prev_pos;
};

#endif //CAMERA_H_