#pragma once
#include <glm\glm.hpp>
#include "../Rendering/Geometry.h"
#include <glm/gtc/matrix_transform.hpp>

class RObject
{
protected:
	glm::vec3 position;
	Geometry* _geometry;
	virtual void draw() = 0;
	virtual void setPosition(glm::vec3 position) = 0;
public:
	RObject();
	~RObject();
};