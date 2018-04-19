#pragma once
#include "RUnit.h"
#include "Camera.h"

class RPlayer : public RUnit
{
private:
	double _speed;
	Camera* _camera;
	float _yaw;
	float _pitch;
	const float _mouse_speed = 0.3f;
public:
	RPlayer();
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera);
	~RPlayer();
	void setTransformation(glm::mat4 transformation) override;
	void move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
};

