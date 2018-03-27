#include "Camera.h"

Camera::~Camera()
{
}

Camera::Camera(float fov, float aspect, float nearZ, float farZ, int window_height, int window_width)
{
	_viewMatrix = glm::mat4(1);
	_projMatrix = glm::perspective(fov, aspect, nearZ, farZ);
	_window_height = window_height;
	_window_width = window_width;
	_xmid = window_width * 0.5;
	_ymid = window_height * 0.5;
	_fov = fov;
	_speed = 3.0f;
	_mouse_speed = 0.005f;
	_position = glm::vec3(0.0, 0.0, 4.0f);
	_viewMatrix = glm::translate(_viewMatrix, _position);
}

glm::mat4 Camera::getViewProjectionMatrix()
{
	glm::mat4 view;
	return glm::mat4(_projMatrix * glm::inverse(_viewMatrix));
}
