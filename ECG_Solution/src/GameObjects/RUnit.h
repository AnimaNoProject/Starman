#pragma once
#include "RObject.h"

class RUnit : public RObject
{
public:
	RUnit(Geometry* geometry);
	RUnit();
	~RUnit();
	void draw();
	void setPosition(glm::vec3 position);
};

