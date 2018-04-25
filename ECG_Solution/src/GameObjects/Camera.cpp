#include "Camera.h"

Camera::~Camera()
{
}

mat4 Camera::lookAt(vec3 const & eye, vec3 const & center, vec3 const & up)
{
	vec3  f = normalize(center - eye);
	vec3  u = normalize(up);
	vec3  s = normalize(cross(f, u));
	u = cross(s, f);

	mat4 result(1);
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

Camera::Camera(float fov, float aspect, float nearZ, float farZ) : _position(0.0, 0.0, -5.0f), _yaw(0.0f), _pitch(0.0f), _viewMatrix(1)
{
	_projMatrix = perspective(fov, aspect, nearZ, farZ);
	_fov = fov;
}

mat4 Camera::getViewProjectionMatrix()
{
	return _projMatrix * _viewMatrix;
}

void Camera::update(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	_yaw += _mouse_speed * deltaTime * x;
	_pitch += _mouse_speed * deltaTime * y;

	vec3 v_dir(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);

	vec3 v_right(
		sin(_yaw - pi<float>() / 2),
		0,
		cos(_yaw - pi<float>() / 2)
	);

	vec3 v_up(glm::cross(v_right, v_dir));

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

vec3 Camera::getPosition()
{
	return _position;
}

void Camera::setSpeed(float speed)
{
	_speed = speed;
}

float Camera::getYaw()
{
	return _yaw;
}

float Camera::getPitch()
{
	return _pitch;
}