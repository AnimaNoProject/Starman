#pragma once
#include "RUnit.h"
#include "Camera.h"

class RPlayer : public RUnit
{
private:
	double _speed;
	Camera* _camera;
public:
	RPlayer();
	//RPlayer(Geometry* geometry);
	//RPlayer(Geometry* geometry, Camera* camera);
	RPlayer(Model* model);
	RPlayer(Model* model, Camera* camera);
	~RPlayer();
	void setPosition(glm::vec3 position) override;
	void move(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
};

