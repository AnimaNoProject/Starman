#pragma once
#include "glm/glm.hpp"

class Plane
{
	glm::vec3 normal, point;
	float d;
public:
	Plane(glm::vec3 &normal, glm::vec3 &point);
	Plane();
	~Plane();
	float distance(glm::vec3 point);
};

