#pragma once
#include "RObject.h"

class RUnit : public RObject
{
private:
	std::vector<RUnit> children;
protected:
	float deltaTime;
	mat4 transformation;
	mat4 _movement;
public:
	RUnit(Model* model);
	RUnit();
	~RUnit();
	void draw();
	void setTransformation(mat4 transformation);
	void addChild(RUnit unit);
	void setTime(float deltaTime);
	void setMovement(vec3 translation, vec3 rotation);
};

