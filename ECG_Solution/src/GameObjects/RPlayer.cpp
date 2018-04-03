#include "RPlayer.h"

RPlayer::RPlayer()
{
}

RPlayer::RPlayer(Geometry * geometry)
{
	_geometry = geometry;
}

RPlayer::RPlayer(Geometry* geometry, Camera* camera) : _speed(0)
{
	_geometry = geometry;
	_camera = camera;
	_position = glm::vec3(0.0f, 0.0f, -10.0f);
}

RPlayer::~RPlayer()
{
}

void RPlayer::setTransformation(glm::mat4 transformation)
{
	_geometry->setTransformMatrix(transformation);
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	if (up)
		(_speed >= 25) ? _speed = 25 : _speed += 0.0025;
	else if (down)
		(_speed <= -25) ? _speed = -25 : _speed -= 0.0025;

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

	_position += v_dir * (float)(deltaTime * _speed);

	if (right)
		_position += v_right * (float)(deltaTime * _speed);
	else if (left)
		_position -= v_right * (float)(deltaTime * _speed);

	_camera->setSpeed(_speed);

	_geometry->setTransformMatrix(glm::translate(_position) * glm::rotate(_yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(-_pitch, glm::vec3(1.0f, 0.0f, 0.0f)));

	_camera->update(x, y, up, down, left, right, deltaTime);
}
