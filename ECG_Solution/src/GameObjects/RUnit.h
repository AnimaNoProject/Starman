#pragma once
#include "RObject.h"

class RUnit : public RObject
{
private:
	std::vector<RUnit> children;

public:
	RUnit(Model* model);
	RUnit();
	~RUnit();
	void draw();
	void setPosition(glm::vec3 position);
	void addChild(RUnit unit);
};

