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
}

RPlayer::~RPlayer()
{
}

void RPlayer::setPosition(glm::vec3 position)
{
	_geometry->setTransformMatrix(glm::translate(glm::mat4(1), position));
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	if (up)
		(_speed > 25) ? _speed = 25 : _speed += 0.25;
	if (down)
		(_speed < -25) ? _speed = -25 : _speed -= 0.25;

	_camera->update(x, y, up, down, left, right, deltaTime);
	setPosition(_camera->getPosition());
}
