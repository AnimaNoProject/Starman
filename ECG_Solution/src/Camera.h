#pragma once
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
protected:
	glm::mat4 _viewMatrix, _projMatrix;
	int _xmid, _ymid, _window_height, _window_width;
	float _fov, _speed, _mouse_speed;
	// position
	glm::vec3 _position;

public:
	Camera(float fov, float aspect, float nearZ, float farZ, int window_height, int window_width);
	~Camera();
	glm::mat4 getViewProjectionMatrix();
};

