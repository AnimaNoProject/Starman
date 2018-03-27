#include "Camera.h"

Camera::~Camera()
{
}

glm::mat4 Camera::lookAt(glm::vec3 const & eye, glm::vec3 const & center, glm::vec3 const & up)
{
	glm::vec3  f = glm::normalize(center - eye);
	glm::vec3  u = glm::normalize(up);
	glm::vec3  s = glm::normalize(cross(f, u));
	u = cross(s, f);

	glm::mat4 result(1);
	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;
	result[3][0] = -dot(s, eye);
	result[3][1] = -dot(u, eye);
	result[3][2] = dot(f, eye);
	return result;
}

Camera::Camera(float fov, float aspect, float nearZ, float farZ, int window_height, int window_width)
{
	_viewMatrix = glm::mat4(1);
	_projMatrix = glm::perspective(fov, aspect, nearZ, farZ);
	_window_height = window_height;
	_window_width = window_width;
	_fov = fov;
	_speed = 3.0f;
	_mouse_speed = 0.005f;
	_position = glm::vec3(0.0, 0.0, 4.0f);
	_viewMatrix = glm::translate(_viewMatrix, _position);
	_yaw = 3.0f;
	_pitch = 0.0f;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
	return glm::mat4(_projMatrix * _viewMatrix);
}

void Camera::update(float xpos, float ypos, float deltaTime)
{
	_pitch += _mouse_speed * deltaTime * float(_window_width / 2 - xpos);
	_yaw += _mouse_speed * deltaTime * float(_window_height / 2 - ypos);
	glm::vec3 direction(
		cos(_yaw) * sin(_pitch),
		sin(_yaw),
		cos(_yaw) * cos(_pitch)
	);

	glm::vec3 right = glm::vec3(
		sin(_pitch - 3.14f / 2.0f),
		0,
		cos(_pitch - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	_viewMatrix = lookAt(_position, _position + direction, up);
}