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

Camera::Camera(float fov, float aspect, float nearZ, float farZ, int window_height, int window_width) : _position(0.0, 0.0, 3.0f), _yaw(3.0f), _pitch(0.0f), _viewMatrix(1)
{
	_projMatrix = glm::perspective(fov, aspect, nearZ, farZ);
	_window_height = window_height;
	_window_width = window_width;
	_fov = fov;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
	return glm::mat4(_projMatrix * _viewMatrix);
}

void Camera::update(float xpos, float ypos, bool up, bool down, bool left, bool right, float deltaTime)
{
	//_pitch += _mouse_speed * deltaTime * float((_window_width / 2) - xpos);
	_pitch += _mouse_speed * deltaTime * float(xpos - (_window_width /2));
	_yaw += _mouse_speed * deltaTime * float(_window_height / 2 - ypos);

	glm::vec3 v_dir(
		cos(_yaw) * sin(_pitch),
		sin(_yaw),
		cos(_yaw) * cos(_pitch)
	);

	glm::vec3 v_right(
		sin(_pitch - glm::pi<float>() * 0.5f),
		0,
		cos(_pitch - glm::pi<float>() * 0.5f)
	);

	glm::vec3 v_up(glm::cross(v_right, v_dir));

	
	if (up)
		_position += v_dir * deltaTime * _speed;
	if (down)
		_position -= v_dir * deltaTime * _speed;
	if (right)
		_position += v_right * deltaTime * _speed;
	if (left)
		_position -= v_right * deltaTime * _speed;

	_viewMatrix = lookAt(_position, _position + v_dir, v_up);
}