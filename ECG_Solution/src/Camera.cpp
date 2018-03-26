#include "Camera.h"

Camera::~Camera()
{
}

Camera::Camera(float fov, float aspect, float nearZ, float farZ)
{
	_viewMatrix = glm::mat4(1);
	_projMatrix = glm::perspective(fov, aspect, nearZ, farZ);
}

glm::mat4 Camera::getViewProjectionMatrix()
{
	return glm::mat4(_projMatrix * glm::inverse(_viewMatrix));
}

void Camera::move(glm::mat4 transformation)
{
	_viewMatrix = transformation * _viewMatrix;
}