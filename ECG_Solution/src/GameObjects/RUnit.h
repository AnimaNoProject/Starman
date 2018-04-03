#pragma once
#include "RObject.h"

class RUnit : public RObject
{
private:
	std::vector<RUnit> children;
protected:
	float deltaTime;
	glm::mat4 transformation;
public:
	RUnit(Geometry* geometry);
	RUnit();
	~RUnit();
	void draw();
	void setTransformation(glm::mat4 transformation);
	void addChild(RUnit unit);
	void setTime(float deltaTime);
	void setMovement(glm::vec3 translation, glm::vec3 rotation);
};

