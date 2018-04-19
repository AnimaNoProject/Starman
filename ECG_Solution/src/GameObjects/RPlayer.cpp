#include "RPlayer.h"

RPlayer::RPlayer()
{
}

RPlayer::RPlayer(Model* model)
{
	_model = model;
}

RPlayer::RPlayer(Model* model, Camera* camera) : _speed(0)
{
	_model = model;
	_camera = camera;
}

RPlayer::~RPlayer()
{
}

void RPlayer::setPosition(vec3 position)
{
	_model->setTransformMatrix(glm::translate(glm::mat4(1), position));
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	_model->resetModelMatrix();
	if (up)
		(_speed >= 25) ? _speed = 25 : _speed += 0.025;
	else if (down)
		(_speed <= -25) ? _speed = -25 : _speed -= 0.025;

	_camera->setSpeed(_speed);

	_camera->update(x, y, up, down, left, right, deltaTime);
	_model->resetModelMatrix();

	mat4 transformationMatrix = translate(_camera->getPosition()) * rotate(-_camera->getPitch(), vec3(1.0f, 0.0f, 0.0f)) * rotate(_camera->getYaw(), vec3(0.0f, 1.0f, 0.0f));
	_model->transform(transformationMatrix);
}