#include "RPlayer.h"

RPlayer::RPlayer()
{
}

RPlayer::RPlayer(Model* model)
{
	_model = model;
}

RPlayer::RPlayer(Model* model, Camera* camera, _Shader* shader) : _speed(0), _real_speed(0), _health(100)
{
	_model = model;
	_model->transform(translate(mat4(1), vec3(0.0f, -1.5f, 5.5f)));
	_camera = camera;
	_shader = shader;
	_position = vec3(0.0f, 0.0f, 5.0f);
	_shot = new Model("assets/objects/starman_ship/shots.obj", _shader);
	timepassedR = timepassedL = 0;
}

RPlayer::~RPlayer()
{
}

void RPlayer::move(float x, float y, bool up, bool down, bool left, bool right, bool shootL, bool shootR, float deltaTime)
{
	if (up)
		(_real_speed >= 25) ? _real_speed = 25 : _real_speed += 5 * deltaTime;
	else if (down)
		(_real_speed <= -25) ? _real_speed = -25 : _real_speed -= 5 * deltaTime;

	_speed = (floor((_real_speed * 2) + 0.5) / 2);

	_yaw += _mouse_speed * deltaTime * x;
	_pitch += _mouse_speed * deltaTime * y;

	_dir = vec3(
		cos(_pitch) * sin(_yaw),
		sin(_pitch),
		cos(_pitch) * cos(_yaw)
	);

	_right = vec3(
		sin(_yaw - pi<float>() / 2),
		0,
		cos(_yaw - pi<float>() / 2)
	);

	_up = vec3(glm::cross(_right, _dir));

	_position += _dir * (float)(deltaTime * _speed);

	if (right)
		_position += _right * (float)(deltaTime * _speed);
	else if (left)
		_position -= _right * (float)(deltaTime * _speed);

	_camera->setSpeed(_speed);

	_model->setTransformMatrix(translate(_position) * rotate(_yaw, vec3(0.0f, 1.0f, 0.0f)) * rotate(-_pitch, vec3(1.0f, 0.0f, 0.0f)));

	_camera->update(x, y, up, down, left, right, deltaTime);

	for (int i = 0; i < this->shots.size(); i++)
	{
			this->shots.at(i)->update(deltaTime);
	}

	for (int i = shots.size() - 1; i >= 0; i--)
	{
		if (shots.at(i)->_toofar)
			shots.erase(shots.begin()+i);
	}

	if(shootL || shootR)
		shoot(deltaTime, shootL, shootR);
}

void RPlayer::shoot(float deltaTime, bool shootL, bool shootR)
{
	timepassedL += deltaTime;
	timepassedR += deltaTime;

	vec3 dir = normalize(_dir);
	vec3 up = normalize(_up);
	vec3 right = normalize(_right);

	if (timepassedL > cooldown && shootL)
	{
		shots.push_back(new Shots(_shot, _dir, _position + (8.5f*dir - 3.2f*up + 4.5f*right)));
		shots.push_back(new Shots(_shot, _dir, _position + (8.5f*dir - 1.5f*up + 4.5f*right)));
		timepassedL = 0;
	}

	if (timepassedR > cooldown && shootR)
	{
		shots.push_back(new Shots(_shot, _dir, _position + (8.5f*dir - 3.2f*up - 4.5f*right)));
		shots.push_back(new Shots(_shot, _dir, _position + (8.5f*dir - 1.5f*up - 4.5f*right)));
		timepassedR = 0;
	}
}

long RPlayer::draw()
{
	long triangles = 0;
	_model->Draw();
	for (int i = 0; i < this->shots.size(); i++)
	{
		if (this->shots.at(i) != nullptr)
			triangles += this->shots.at(i)->draw();
	}
	return triangles;
}
