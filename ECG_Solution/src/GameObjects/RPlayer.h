#pragma once
#include "RUnit.h"
#include "Camera.h"
#include "../Rendering/Model.h"

class RPlayer
{
private:
	std::vector<Shots*> shots;
	vec3 _position;
	Model* _model;
	double _speed;
	double _real_speed;
	Camera* _camera;
	float _yaw;
	float _pitch;
	const float _mouse_speed = 2.0f;
	vec3 _right;
	vec3 _up;
	vec3 _dir;
	_Shader* _shader;
	Model* _shot;
	float timepassed;
	float cooldown = 1.0f;
public:
	RPlayer();
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera, _Shader* shader);
	~RPlayer();
	void move(float x, float y, bool up, bool down, bool left, bool right, bool shooting, float deltaTime);
	void shoot(float deltaTime);
	void draw();
};

