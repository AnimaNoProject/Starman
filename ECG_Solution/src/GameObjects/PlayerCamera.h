#pragma once
#include "Camera.h"

class PlayerCamera : public Camera
{

public:
	PlayerCamera(float fov, float aspect, float nearZ, float farZ) : Camera(fov, aspect, nearZ, farZ) {};
	~PlayerCamera();
	void update(float x, float y, bool up, bool down, bool left, bool right, float deltaTime);
};

