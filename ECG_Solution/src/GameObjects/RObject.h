#pragma once
#include <glm\glm.hpp>
#include "../Rendering/Geometry.h"
#include <glm/gtc/matrix_transform.hpp>

class RObject
{
protected:
	glm::vec3 _position;
	Geometry* _geometry;
	virtual void draw() = 0;
	virtual void setTransformation(glm::mat4 transformation) = 0;
public:
	RObject();
	~RObject();
};