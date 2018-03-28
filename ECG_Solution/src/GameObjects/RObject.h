#pragma once
#include <glm\glm.hpp>
#include "../Rendering/Geometry.h"
#include <glm/gtc/matrix_transform.hpp>

class RObject
{
protected:
	glm::vec3 position;
	Geometry* mesh;
	virtual void draw();
	virtual void setPosition(glm::vec3 position);
public:
	RObject();
	~RObject();
};