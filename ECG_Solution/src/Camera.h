#pragma once
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

class Camera
{
protected:
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;

public:
	Camera(float fov, float aspect, float nearZ, float farZ);
	~Camera();
	glm::mat4 getViewProjectionMatrix();
};

