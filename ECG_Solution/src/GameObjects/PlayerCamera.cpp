#include "PlayerCamera.h"

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::update(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	_yaw += _mouse_speed * deltaTime * x;
	_pitch += _mouse_speed * deltaTime * y;

	glm::vec3 v_dir(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);

	glm::vec3 v_right(
		sin(_yaw - glm::pi<float>() / 2),
		0,
		cos(_yaw - glm::pi<float>() / 2)
	);

	glm::vec3 v_up(glm::cross(v_right, v_dir));

	_position += v_dir * deltaTime * _speed;

	if (right)
		_position += v_right * deltaTime * _speed;
	else if (left)
		_position -= v_right * deltaTime * _speed;

	glm::vec3 temp_pos = _position - v_dir * 2.0f + v_up * 2.0f;

	_viewMatrix = lookAt(temp_pos, temp_pos + v_dir, v_up);
}