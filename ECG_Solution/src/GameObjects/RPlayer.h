#pragma once
#include "RUnit.h"
#include "Camera.h"
#include "../Rendering/Model.h"

class RPlayer
{
private:
	vec3 _position;
	Model* _model;
	double _speed;
	double _real_speed;
	Camera* _camera;
	float _yaw;
	float _pitch;
	const float _mouse_speed = 0.0125f;
public:
	RPlayer();
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera);
	~RPlayer();
	void move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
	void draw();
};

