#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
protected:
	glm::mat4 _viewMatrix, _projMatrix;
	float _pitch, _yaw, _fov;

	float _speed = 2.00f;
	const float _mouse_speed = 0.3f;

	glm::vec3 _position;
	glm::mat4 lookAt(glm::vec3  const & eye, glm::vec3  const & center, glm::vec3  const & up);
public:
	Camera(float fov, float aspect, float nearZ, float farZ);
	~Camera();
	glm::mat4 getViewProjectionMatrix();
	virtual void update(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
	glm::vec3 getPosition();
	void setSpeed(float speed);
	float getYaw();
	float getPitch();
	void resetCamera();
};