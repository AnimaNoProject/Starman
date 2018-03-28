#pragma once
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
protected:
	glm::mat4 _viewMatrix, _projMatrix;
	int  _window_height, _window_width;
	float _pitch, _yaw, _fov;

	const float _speed = 0.0005f;
	const float _mouse_speed = 0.0005f;

	glm::vec3 _position;
	glm::mat4 lookAt(glm::vec3  const & eye, glm::vec3  const & center, glm::vec3  const & up);

public:
	Camera(float fov, float aspect, float nearZ, float farZ, int window_height, int window_width);
	~Camera();
	glm::mat4 getViewProjectionMatrix();
	void update(float xpos, float ypos, bool up, bool down, bool left, bool right, float deltaTime);
};

