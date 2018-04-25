#pragma once
#include "RUnit.h"
#include "Camera.h"

class RPlayer : public RObject
{
private:
	double _speed;
	double _real_speed;
	Camera* _camera;
	float _yaw;
	float _pitch;
	const float _mouse_speed = 0.3f;
public:
	RPlayer();
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera);
	~RPlayer();
	void setTransformation(mat4 transformation) override;
	void move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
	void draw();
};

