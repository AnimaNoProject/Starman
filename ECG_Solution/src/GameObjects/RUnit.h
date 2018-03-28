#pragma once
#include "RObject.h"

class RUnit : public RObject
{
public:
	RUnit(Geometry* geom);
	~RUnit();
	void draw();
	void setPosition(glm::vec3 position);
};

