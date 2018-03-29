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
	_geometry->resetModelMatrix();
	if (up)
		(_speed >= 25) ? _speed = 25 : _speed += 0.0025;
	else if (down)
		(_speed <= -25) ? _speed = -25 : _speed -= 0.0025;

	_camera->setSpeed(_speed);

	_camera->update(x, y, up, down, left, right, deltaTime);
	_geometry->resetModelMatrix();
	_geometry->transform(glm::translate(_camera->getPosition()) * glm::rotate(-_camera->getPitch(), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(_camera->getYaw(), glm::vec3(0.0f, 1.0f, 0.0f)));
}
