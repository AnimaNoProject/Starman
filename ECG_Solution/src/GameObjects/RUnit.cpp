#include "RUnit.h"

RUnit::RUnit()
{
	mesh = Geometry(glm::mat4(1), Geometry::createTestObject(2.0f, 2.0f, 2.0f));
}

RUnit::~RUnit()
{
}

void RUnit::draw()
{
	mesh.draw();
}

void RUnit::setPosition(glm::vec3 position)
{
	mesh.transform(glm::translate(glm::mat4(1), position));
}
