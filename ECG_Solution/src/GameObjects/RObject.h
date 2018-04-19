#pragma once
#include <glm\glm.hpp>
#include "../Rendering/Model.h"
#include <glm/gtc/matrix_transform.hpp>

class RObject
{
protected:
	glm::vec3 position;
	Model* _model;
	virtual void draw() = 0;
	virtual void setPosition(glm::vec3 position) = 0;
public:
	RObject();
	~RObject();
};