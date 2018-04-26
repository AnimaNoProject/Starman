#include "RPlayer.h"

RPlayer::RPlayer()
{
}

RPlayer::RPlayer(Model* model)
{
	_model = model;
}

RPlayer::RPlayer(Model* model, Camera* camera) : _speed(0), _real_speed(0)
{
	_model = model;
	_camera = camera;
	_position = vec3(0.0f, 0.0f, -5.0f);
}

RPlayer::~RPlayer()
{
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime)
{
	if (up)
		(_real_speed >= 25) ? _real_speed = 25 : _real_speed += 1 * deltaTime;
	else if (down)
		(_real_speed <= -25) ? _real_speed = -25 : _real_speed -= 1 * deltaTime;

	_speed = (floor((_real_speed * 2) + 0.5) / 2);

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

	_position += v_dir * (float)(deltaTime * _speed);

	if (right)
		_position += v_right * (float)(deltaTime * _speed);
	else if (left)
		_position -= v_right * (float)(deltaTime * _speed);

	_camera->setSpeed(_speed);

	_model->setTransformMatrix(mat4(1) * translate(_position) * rotate(_yaw, vec3(0.0f, 1.0f, 0.0f)) * rotate(-_pitch, vec3(1.0f, 0.0f, 0.0f)));

	_camera->update(x, y, up, down, left, right, deltaTime);
}

void RPlayer::draw()
{
	_model->Draw();
}
