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
	Camera* _camera;
	float _yaw;
	float _pitch;
	const float _mouse_speed = 2.0f;
	vec3 _right;
	vec3 _up;
	vec3 _dir;
	_Shader* _shader;
	Model* _shot;
	float timepassedL;
	float timepassedR;
	float cooldown = 0.25f;
public:
	RPlayer();
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera, _Shader* shader);
	~RPlayer();
	double _real_speed;
	int _health;
	void move(float x, float y, bool up, bool down, bool left, bool right, bool shootR, bool shootL, float deltaTime);
	void shoot(float deltaTime, bool shootL, bool shootR);
	void draw();
	vec3 getPosition() { return _position; }
};

