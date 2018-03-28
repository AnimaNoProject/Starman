#pragma once
#include "RObject.h"

class RUnit : public RObject
{
public:
	RUnit();
	~RUnit();
	void draw();
	void setPosition(glm::vec3 position);
};

